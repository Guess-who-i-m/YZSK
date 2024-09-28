# # 接下来是模型的部分
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import math

import torch
import torch.nn as nn 
import torch.nn.functional as F

import torch.hub
from torch.utils.data import Subset

import torchvision
from torchvision import datasets, models, transforms
from torchvision.models.feature_extraction import get_graph_node_names
from torchvision.models.feature_extraction import create_feature_extractor

import torchaudio.functional as F
import torchaudio.transforms as T

from torchinfo import summary

from scipy.io import wavfile

from typing import Type, Any, Callable, Union, List, Optional
from torch import Tensor

import numpy as np
from PIL import Image

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

def visualize_data(input, title=None):
    input = input.numpy().transpose((1, 2, 0))
    mean = np.array([0.485, 0.456, 0.406])
    std = np.array([0.229, 0.224, 0.225])
    input = std * input + mean
    input = np.clip(input, 0, 1)
    plt.imshow(input)

    if title is not None:
       plt.title(None) # remove titles

    plt.pause(0.001)  ## Here we are pausing a bit so that plots are updated


class SEResNet(nn.Module):
    def __init__(self, block, layers, num_classes=1000):
        self.inplanes = 64
        super(SEResNet, self).__init__()
        self.conv1 = nn.Conv2d(3, 64, kernel_size=7, stride=2, padding=3,
                               bias=False)
        self.bn1 = nn.BatchNorm2d(64)
        self.relu = nn.ReLU(inplace=True)
        self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        self.layer1 = self._make_layer(block, 64, layers[0])
        self.layer2 = self._make_layer(block, 128, layers[1], stride=2)
        self.layer3 = self._make_layer(block, 256, layers[2], stride=2)
        self.layer4 = self._make_layer(block, 512, layers[3], stride=2)
        self.avgpool = nn.AvgPool2d(7)
        self.fc = nn.Linear(512 * block.expansion, num_classes)

        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                n = m.kernel_size[0] * m.kernel_size[1] * m.out_channels
                m.weight.data.normal_(0, math.sqrt(2. / n))
            elif isinstance(m, nn.BatchNorm2d):
                m.weight.data.fill_(1)
                m.bias.data.zero_()

    def _make_layer(self, block, planes, blocks, stride=1):
        downsample = None
        if stride != 1 or self.inplanes != planes * block.expansion:
            downsample = nn.Sequential(
                nn.Conv2d(self.inplanes, planes * block.expansion,
                          kernel_size=1, stride=stride, bias=False),
                nn.BatchNorm2d(planes * block.expansion),
            )

        layers = []
        layers.append(block(self.inplanes, planes, stride, downsample))
        self.inplanes = planes * block.expansion
        for i in range(1, blocks):
            layers.append(block(self.inplanes, planes))

        return nn.Sequential(*layers)

    def forward(self, x):
        x = self.conv1(x)
        x = self.bn1(x)
        x = self.relu(x)
        x = self.maxpool(x)

        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)

        x = self.avgpool(x)
        x = x.view(x.size(0), -1)
        x = self.fc(x)

        return x


class Bottleneck(nn.Module): # what is this ....
    expansion = 4

    def __init__(self, inplanes, planes, stride=1, downsample=None):
        super(Bottleneck, self).__init__()
        self.conv1 = nn.Conv2d(inplanes, planes, kernel_size=1, bias=False)
        self.bn1 = nn.BatchNorm2d(planes)
        self.conv2 = nn.Conv2d(planes, planes, kernel_size=3, stride=stride,
                               padding=1, bias=False)
        self.bn2 = nn.BatchNorm2d(planes)
        self.conv3 = nn.Conv2d(planes, planes * 4, kernel_size=1, bias=False)
        self.bn3 = nn.BatchNorm2d(planes * 4)
        self.relu = nn.ReLU(inplace=True)
        # SE
        self.global_pool = nn.AdaptiveAvgPool2d(1)
        self.conv_down = nn.Conv2d(
            planes * 4, planes // 4, kernel_size=1, bias=False)
        self.conv_up = nn.Conv2d(
            planes // 4, planes * 4, kernel_size=1, bias=False)
        self.sig = nn.Sigmoid()
        # Downsample
        self.downsample = downsample
        self.stride = stride

    def forward(self, x):
        residual = x

        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)

        out = self.conv2(out)
        out = self.bn2(out)
        out = self.relu(out)

        out = self.conv3(out)
        out = self.bn3(out)

        out1 = self.global_pool(out)
        out1 = self.conv_down(out1)
        out1 = self.relu(out1)
        out1 = self.conv_up(out1)
        out1 = self.sig(out1)

        if self.downsample is not None:
            residual = self.downsample(x)

        res = out1 * out + residual
        res = self.relu(res)

        return res


def se_resnet50(**kwargs):
    """Constructs a SE-ResNet-50 model.
    Args:
        num_classes = 1000 (default)
    """
    model = SEResNet(Bottleneck, [3, 4, 6, 3], **kwargs)
    return model


def se_resnet101(**kwargs):
    """Constructs a SE-ResNet-101 model.
    Args:
        num_classes = 1000 (default)
    """
    model = SEResNet(Bottleneck, [3, 4, 23, 3], **kwargs)
    return model


def se_resnet152(**kwargs):
    """Constructs a SE-ResNet-152 model.
    Args:
        num_classes = 1000 (default)
    """
    model = SEResNet(Bottleneck, [3, 8, 36, 3], **kwargs)
    return model


class CNNLSTMClassifier(nn.Module):
    def __init__(self, input_size, hidden_size, num_layers, batch_size, no_classes, num_directions=1):
        super(CNNLSTMClassifier, self).__init__()
        self.conv1 = nn.Sequential(
            nn.Conv2d(in_channels=input_size, out_channels=64, kernel_size=1),
            nn.ReLU(),
        )
        self.conv2 = nn.Sequential(
            nn.Conv2d(64, 512, kernel_size=1),
            nn.ReLU(),
        )
        
        self.lstm = nn.LSTM(input_size=512, hidden_size=hidden_size, num_layers=num_layers,
                            bidirectional=(num_directions==2), batch_first=True)

        self.hidden = self.init_hidden(batch_size, hidden_size, num_directions, num_layers)
        self.linear2 = nn.Linear(hidden_size*num_directions, no_classes)

    def init_hidden(self, batch_size, hidden_size, num_directions, num_layers):
        return (torch.zeros(num_directions*num_layers, batch_size, hidden_size).to(device),
                torch.zeros(num_directions*num_layers, batch_size, hidden_size).to(device))

    def forward(self, x):
        print(f'x shape pre-conv1: {x.shape}')
        x = self.conv1(x)
        print(f'x shape post-conv1: {x.shape}')
        x = self.conv2(x)
        print(f'x shape post-conv2: {x.shape}')
        
        x = x.view(x.size(0), x.size(2) * x.size(3), -1)  # 调整形状为 [batch_size, sequence_length, features]
        print(f'x shape pre-lstm: {x.shape}')
        
        x, (hn, cn) = self.lstm(x, self.hidden)
        y = self.linear2(x[:, -1, :])
        return y, x, hn, cn


class SELayer(nn.Module):
    """From https://github.com/moskomule/senet.pytorch"""

    def __init__(self, channel, reduction=16):
        super().__init__()
        self.avg_pool = nn.AdaptiveAvgPool2d(1)
        self.fc = nn.Sequential(
            nn.Linear(channel, channel // reduction, bias=False),
            nn.ReLU(inplace=True),
            nn.Linear(channel // reduction, channel, bias=False),
            nn.Sigmoid(),
        )

    def forward(self, x):
        b, c, _, _ = x.size()
        y = self.avg_pool(x).view(b, c)
        y = self.fc(y).view(b, c, 1, 1)
        return x * y.expand_as(x)


def conv1x1(in_planes: int, out_planes: int, stride: int = 1) -> nn.Conv2d:
    """1x1 convolution"""
    return nn.Conv2d(in_planes, out_planes, kernel_size=1, stride=stride, bias=False)


def conv3x3(in_planes: int, out_planes: int, stride: int = 1, groups: int = 1, dilation: int = 1) -> nn.Conv2d:
    """3x3 convolution with padding"""
    return nn.Conv2d(
        in_planes,
        out_planes,
        kernel_size=3,
        stride=stride,
        padding=dilation,
        groups=groups,
        bias=False,
        dilation=dilation,
    )


class SEBasicBlock(models.resnet.BasicBlock):
    def __init__(
        self,
        inplanes: int,
        planes: int,
        stride: int = 1,
        downsample: Optional[nn.Module] = None,
        groups: int = 1,
        base_width: int = 64,
        dilation: int = 1,
        norm_layer: Optional[Callable[..., nn.Module]] = None,
        reduction: int = 16,
    ) -> None:
        super().__init__(
            inplanes=inplanes,
            planes=planes,
            stride=stride,
            downsample=downsample,
            groups=groups,
            base_width=base_width,
            dilation=dilation,
            norm_layer=norm_layer,
        )
        self.se = SELayer(planes, reduction)

    def forward(self, x: Tensor) -> Tensor:
        residual = x
        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)

        out = self.conv2(out)
        out = self.bn2(out)
        out = self.se(out)

        if self.downsample is not None:
            residual = self.downsample(x)

        out += residual
        out = self.relu(out)

        return out


class SEBottleneck(nn.Module):
    # Bottleneck in torchvision places the stride for downsampling at 3x3 convolution(self.conv2)
    # while original implementation places the stride at the first 1x1 convolution(self.conv1)
    # according to "Deep residual learning for image recognition"https://arxiv.org/abs/1512.03385.
    # The ResNet V1.5 and improves accuracy according to
    # https://ngc.nvidia.com/catalog/model-scripts/nvidia:resnet_50_v1_5_for_pytorch.

    expansion: int = 4

    def __init__(
        self,
        inplanes: int,
        planes: int,
        stride: int = 1,
        downsample: Optional[nn.Module] = None,
        groups: int = 1,
        base_width: int = 64,
        dilation: int = 1,
        norm_layer: Optional[Callable[..., nn.Module]] = None,
        reduction: int = 16,
    ) -> None:
        """
        Note: Not inherited from torchvision.models.resnet.Bottleneck.__init__ due to replacing resnetv1 with resnetv1.5.
        Args:
            inplanes (int): _description_
            planes (int): _description_
            stride (int, optional): _description_. Defaults to 1.
            downsample (Optional[nn.Module], optional): _description_. Defaults to None.
            groups (int, optional): _description_. Defaults to 1.
            base_width (int, optional): _description_. Defaults to 64.
            dilation (int, optional): _description_. Defaults to 1.
            norm_layer (Optional[Callable[..., nn.Module]], optional): _description_. Defaults to None.
            reduction (int, optional): _description_. Defaults to 16.
        """
        super().__init__()
        if norm_layer is None:
            norm_layer = nn.BatchNorm2d
        width = int(planes * (base_width / 64.0)) * groups
        # Both self.conv1 and self.downsample layers downsample the input when stride != 1
        self.conv1 = conv1x1(inplanes, width, stride, groups, dilation)  # Original ResNet implementation
        self.bn1 = norm_layer(width)
        self.conv2 = conv3x3(width, width)
        self.bn2 = norm_layer(width)
        self.conv3 = conv1x1(width, planes * self.expansion)
        self.bn3 = norm_layer(planes * self.expansion)
        self.relu = nn.ReLU(inplace=True)
        self.downsample = downsample
        self.stride = stride
        self.se = SELayer(planes * 4, reduction)

    def forward(self, x: Tensor) -> Tensor:
        residual = x

        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)

        out = self.conv2(out)
        out = self.bn2(out)
        out = self.relu(out)

        out = self.conv3(out)
        out = self.bn3(out)
        out = self.se(out)

        if self.downsample is not None:
            residual = self.downsample(x)

        out += residual
        out = self.relu(out)

        return out


class SEResNet(nn.Module):
    def __init__(
        self,
        block: Type[Union[SEBasicBlock, SEBottleneck]],
        layers: List[int],
        num_classes: int = 1000,
        zero_init_residual: bool = False,
        groups: int = 1,
        width_per_group: int = 64,
        replace_stride_with_dilation: Optional[List[bool]] = None,
        norm_layer: Optional[Callable[..., nn.Module]] = None,
    ) -> None:
        super().__init__()
        if norm_layer is None:
            norm_layer = nn.BatchNorm2d
        self._norm_layer = norm_layer

        self.inplanes = 64
        self.dilation = 1
        if replace_stride_with_dilation is None:
            # each element in the tuple indicates if we should replace
            # the 2x2 stride with a dilated convolution instead
            replace_stride_with_dilation = [False, False, False]
        if len(replace_stride_with_dilation) != 3:
            raise ValueError("replace_stride_with_dilation should be None " f"or a 3-element tuple, got {replace_stride_with_dilation}")
        self.groups = groups
        self.base_width = width_per_group
        self.conv1 = nn.Conv2d(3, self.inplanes, kernel_size=7, stride=2, padding=3, bias=False)
        self.bn1 = norm_layer(self.inplanes)
        self.relu = nn.ReLU(inplace=True)
        self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        self.layer1 = self._make_layer(block, 64, layers[0])
        self.layer2 = self._make_layer(block, 128, layers[1], stride=2, dilate=replace_stride_with_dilation[0])
        self.layer3 = self._make_layer(block, 256, layers[2], stride=2, dilate=replace_stride_with_dilation[1])
        self.layer4 = self._make_layer(block, 512, layers[3], stride=2, dilate=replace_stride_with_dilation[2])
        # self.avgpool = nn.AdaptiveAvgPool2d((1, 1))
        # self.fc = nn.Linear(512 * block.expansion, num_classes)

        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                nn.init.kaiming_normal_(m.weight, mode="fan_out", nonlinearity="relu")
            elif isinstance(m, (nn.BatchNorm2d, nn.GroupNorm)):
                nn.init.constant_(m.weight, 1)
                nn.init.constant_(m.bias, 0)

        # Zero-initialize the last BN in each residual branch,
        # so that the residual branch starts with zeros, and each residual block behaves like an identity.
        # This improves the model by 0.2~0.3% according to https://arxiv.org/abs/1706.02677
        if zero_init_residual:
            for m in self.modules():
                if isinstance(m, SEBottleneck):
                    nn.init.constant_(m.bn3.weight, 0)  # type: ignore[arg-type]
                elif isinstance(m, SEBasicBlock):
                    nn.init.constant_(m.bn2.weight, 0)  # type: ignore[arg-type]

    def _make_layer(
        self,
        block: Type[Union[SEBasicBlock, SEBottleneck]],
        planes: int,
        blocks: int,
        stride: int = 1,
        dilate: bool = False,
    ) -> nn.Sequential:
        norm_layer = self._norm_layer
        downsample = None
        previous_dilation = self.dilation
        if dilate:
            self.dilation *= stride
            stride = 1
        if stride != 1 or self.inplanes != planes * block.expansion:
            downsample = nn.Sequential(
                conv1x1(self.inplanes, planes * block.expansion, stride),
                norm_layer(planes * block.expansion),
            )

        layers = []
        layers.append(block(self.inplanes, planes, stride, downsample, self.groups, self.base_width, previous_dilation, norm_layer))
        self.inplanes = planes * block.expansion
        for _ in range(1, blocks):
            layers.append(
                block(
                    self.inplanes,
                    planes,
                    groups=self.groups,
                    base_width=self.base_width,
                    dilation=self.dilation,
                    norm_layer=norm_layer,
                )
            )

        return nn.Sequential(*layers)

    def _forward_impl(self, x: Tensor) -> Tensor:
        x = self.conv1(x)
        x = self.bn1(x)
        x = self.relu(x)
        x = self.maxpool(x)

        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)

        # x = self.avgpool(x)
        # x = torch.flatten(x, 1)
        # x = self.fc(x)

        return x

    def forward(self, x: Tensor) -> Tensor:
        return self._forward_impl(x)


import torch
from torch import nn
from einops import rearrange


class Attention(nn.Module):
    """From https://github.com/lucidrains/vit-pytorch/blob/main/vit_pytorch/vit.py"""

    def __init__(self, dim, heads=8, dim_head=64, dropout=0.0):
        super().__init__()
        inner_dim = dim_head * heads
        project_out = not (heads == 1 and dim_head == dim)

        self.heads = heads
        self.scale = dim_head ** -0.5

        self.attend = nn.Softmax(dim=-1)
        self.to_qkv = nn.Linear(dim, inner_dim * 3, bias=False)

        self.to_out = nn.Sequential(nn.Linear(inner_dim, dim), nn.Dropout(dropout)) if project_out else nn.Identity()

    def forward(self, x):
        qkv = self.to_qkv(x).chunk(3, dim=-1)
        q, k, v = map(lambda t: rearrange(t, "b n (h d) -> b h n d", h=self.heads), qkv)

        dots = torch.matmul(q, k.transpose(-1, -2)) * self.scale

        attn = self.attend(dots)

        out = torch.matmul(attn, v)
        out = rearrange(out, "b h n d -> b n (h d)")
        return self.to_out(out)


class FluentNetOneModel(nn.Module):
    def __init__(
        self,
        resnet_layers: List[int] = [2] * 8,
        resnet_block=SEBasicBlock,
        bilstm_dropout: float = 0.2,
        attention_dropout: float = 0.0,
        attention_dim_head: int = 128,
        hidden_size: int = 512,
    ):
        super().__init__()
        self.seresnet = SEResNet(resnet_block, layers=resnet_layers, num_classes=1)
        self.bilstm_layer = nn.LSTM(input_size=300, hidden_size=hidden_size, bidirectional=True, batch_first=True, num_layers=2, dropout=bilstm_dropout) # input_size used to be 1024
        self.attention_layer = Attention(dim=1024, dim_head=attention_dim_head, heads=1, dropout=attention_dropout)  # Global attention
        self.fc = nn.Linear(in_features=1024 * hidden_size, out_features=1)
        self.sigmoid = nn.Sigmoid()

    def forward(self, x: torch.Tensor):
        out = self.seresnet(x)
        print(f"Output of SEResNet: {out.shape}")
        
        # batch_size, channels, height, width = out.shape
        # bil_in = out.view(batch_size, height * width, channels)
        bil_in = out.flatten(start_dim=-2, end_dim=-1)
        print(f"Input to LSTM: {bil_in.shape}")
        
        bil_out, _ = self.bilstm_layer(bil_in)
        print(f"Output of LSTM: {bil_out.shape}")
        
        att_out = self.attention_layer(bil_out)
        print(f"Output of Attention: {att_out.shape}")
        
        out = att_out.flatten(1)
        print(f"Flattened output: {out.shape}")
        
        out = self.fc(out)
        print(f"Output of FC: {out.shape}")
        
        out = self.sigmoid(out)
        return out


class FluentNet(nn.Module):
    def __init__(self):
        super().__init__()
        self.sound_rep_model = FluentNetOneModel()
        self.word_rep_model = FluentNetOneModel()
        self.phrase_rep_model = FluentNetOneModel()
        #self.revision_model = FluentNetOneModel()
        #self.interjection_model = FluentNetOneModel()
        self.prolongation_model = FluentNetOneModel()
        self.clean_model = FluentNetOneModel() ## Added myself

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        sound_rep = self.sound_rep_model(x)
        word_rep = self.word_rep_model(x)
        phrase_rep = self.phrase_rep_model(x)
        #revision = self.revision_model(x)
        #interjection = self.interjection_model(x)
        prolongation = self.prolongation_model(x)
        clean = self.clean_model(x) ## Added myself

        #out = torch.stack([sound_rep, word_rep, phrase_rep, revision, interjection, prolongation], axis=-1)
        out = torch.stack([clean, sound_rep, word_rep, phrase_rep, prolongation], axis=-1)
        return out


def graph_spectrogram(wav_file):
    rate, data = wavfile.read(wav_file)
    fig,ax = plt.subplots(1)
    
    # 确保输入数据是单声道
    if len(data.shape) > 1 and data.shape[1] == 2:
        data = data.mean(axis=1)  # 将立体声转换为单声道

    # 调整窗口函数的形状
    window = np.hanning(256)
    window = window.reshape((256, 1))
    
    fig.subplots_adjust(left=0,right=1,bottom=0,top=1)
    ax.axis('off')
    pxx, freqs, bins, im = ax.specgram(x=data, Fs=16000, noverlap=128, NFFT=256, window=window)
    ax.axis('off')
    # fig.savefig('./image.png', dpi=300, frameon='false')
    


# 定义图像变换
preprocess_transform = transforms.Compose([
    transforms.Resize((480, 640)),  # 调整图像大小
    transforms.ToTensor()           # 转换为张量
])

def preprocess(image_path):
    """
    预处理输入图像，确保其符合模型的输入要求
    参数:
    image_path (str): 输入图像的路径
    返回:
    torch.Tensor: 预处理后的图像张量
    """
    image = Image.open(image_path).convert("RGB")  # 打开图像并转换为RGB格式
    image = preprocess_transform(image)            # 应用预处理变换
    return image








