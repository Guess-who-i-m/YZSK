<script setup>

</script>

<template>
  <!-- this is app.vue -->


  <router-view></router-view>
  <van-tabbar route>
  
  <van-tabbar-item replace to="/choosemode" icon="chart-trending-o">训练</van-tabbar-item>
  <van-tabbar-item replace to="/record" icon="records-o">记录</van-tabbar-item>
  <van-tabbar-item replace to="/user" icon="manager-o">我的</van-tabbar-item>
</van-tabbar>
</template>

<style scoped>

</style>
