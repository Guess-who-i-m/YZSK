<script setup>
import { ref } from 'vue';
import { useRouter } from 'vue-router'
import { showConfirmDialog } from 'vant';
import { NoticeBar } from 'vant';
// import {router}
const router=useRouter()
const handleBasic = () => {
    router.push('/choosetext')
    // console.log("自定义");
}
const handleSelf = () => {
  showConfirmDialog({
  title: '创建自定义任务',
  message:
    '确定要创建自定义任务吗？',
})
  .then(() => {
    router.push('/hadchooses')
    // on confirm
  })
  .catch(() => {
    // on cancel
  });
    
    // console.log("自定义");
}
</script>

<template>
  <!-- 选择训练模式
  <van-cell-group inset>
    <van-cell title="单元格" value="内容" label="描述信息"/>
    <van-cell title="单元格" value="内容" label="描述信息" />
  </van-cell-group> -->
  
  <van-nav-bar
  title="训练模式"
/>
<van-notice-bar color="#1989fa" background="#ecf9ff" left-icon="label-o" :scrollable="false">
  <van-swipe
    vertical
    class="notice-swipe"
    :autoplay="3000"
    :touchable="false"
    :show-indicators="false"
  >
    <van-swipe-item>基础训练任务点击选择文本</van-swipe-item>
    <van-swipe-item>自定义训练任务点击直接创建任务</van-swipe-item>
    <!-- <van-swipe-item>基础训练任务使用我们为您提供的训练文本</van-swipe-item> -->
  </van-swipe>
</van-notice-bar>
  <van-card
    class="custom-card"
    desc="使用我们为您提供的文本进行训练"
    title="基础训练模式"
    thumb="https://fastly.jsdelivr.net/npm/@vant/assets/ipad.jpeg"
    @click="handleBasic"
  />
  <van-card
    class="custom-card"
    desc="根据您的需求进行个性化训练"
    title="自定义练习"
    thumb="https://fastly.jsdelivr.net/npm/@vant/assets/ipad.jpeg"
    @click="handleSelf"
  />
</template>

<style scoped>
.custom-card .van-card__title {
  font-size: 30px; /* 调整为你需要的字体大小 */
}
.notice-swipe {
    height: 40px;
    line-height: 40px;
  }
</style>
