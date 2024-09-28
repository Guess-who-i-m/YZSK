<script setup>
import {ref,onMounted} from 'vue'
import { showConfirmDialog } from 'vant';
import axios from 'axios';
import { useRouter } from 'vue-router'
// import axios from 'axios';
import ipAddr from '@/api/index';
import { tabProps } from 'vant';
const router=useRouter()
//数据类型定义
const textData=ref([
{
        "id": 1,
        "textContent": "哦啊啊诶诶啊啊哦哦啊啊诶诶嗒噶嗒噶嗒噶",
        "textType": 0
      },
      {
        "id": 2,
        "textContent": "几个蛋几个蛋几个蛋蛋蛋蛋东东东巴布蛋蛋咚咚咚巴布",
        "textType": 0
      },
      {
        "id": 3,
        "textContent": "标号为零是测试文本",
        "textType": 0
      },
])
const firstValue=ref(0)



//函数定义
const goback=()=>{
    router.push('/choosemode')
}

//获取文本
const getTextData=async()=>{
    const res=await axios.get(`http://${ipAddr}/user/getBasicText`)
    .then((res)=>{
        console.log(res)
        if(res.data.status==200){
            firstValue.value=res.data.data.basictextlist[0].id
            console.log('firstValue.value is ',firstValue.value)
            textData.value=res.data.data.basictextlist
            console.log("tabledata",textData.value)
            firstValue.value=textData.value[0].id
            for(let i=0;i<textData.value.length;i++){
                textData.value[i].id=i+1
                
                if(textData.value[i].textType==21){
                    textData.value[i].textType="单韵母练习"
                }else if(textData.value[i].textType==22){
                    textData.value[i].textType="鼻韵母练习"
                }else if(textData.value[i].textType==23){
                    textData.value[i].textType="复韵母练习"
                }else if(textData.value[i].textType==24){
                    textData.value[i].textType="唇音练习"
                }else if(textData.value[i].textType==25){
                    textData.value[i].textType="舌尖中音练习"
                }else if(textData.value[i].textType==26){
                    textData.value[i].textType="舌根音、舌面音练习"
                }
            }
        }
    }).catch((error)=>{
        console.log("发生异常")
        console.log(error)
    })
}
const handleclick=(index)=>{
    console.log(index)
    const text=textData.value[index-1].textContent
    showConfirmDialog({
  title: '请确认：',
  message:
    '是否选择该文本创建任务吗？',
})
  .then(() => {
    console.log('yes')
    console.log("*******",index)
    router.push({path:'/hadchoose',query:{
        text:text,
        id:index+firstValue.value
    }})
    // on confirm
  })
  .catch(() => {
    console.log('no')
    // on cancel
  });
}

//函数调用
getTextData()
console.log("firstvalue is ",firstValue.value)
</script>

<template>
<van-sticky>
    <van-nav-bar
  title="文本选择"
  left-text="返回"
  left-arrow
  @click-left="goback()"
/>
</van-sticky>

<van-notice-bar color="#1989fa" background="#ecf9ff" left-icon="info-o">
  点击选择你要练习的文本
</van-notice-bar>


<van-cell-group v-for="item in textData" @click="handleclick(item.id)">
  <van-cell
  
  :key="item.id"
  :title="item.id" 
  :value="item.textContent" 
  :label="item.textType" />
  <van-divider
  :style="{ color: '#1989fa', borderColor: '#1989fa', padding: '0 16px' }"
>
  我是分割线
</van-divider>
</van-cell-group>
<van-skeleton title :row="3" />
</template>

