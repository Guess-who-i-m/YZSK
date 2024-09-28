<script setup>
import { ref, onMounted } from 'vue'
import ipAddr from '@/api/index'
import axios from 'axios'
import { useRouter } from 'vue-router'

const router = useRouter()

const tabledata = ref([
  {
    "taskId": "task_1721302568396",
    "createdAt": "2024-07-18T11:36:08.000+00:00",
    "id": 1,
    "taskType": 1
  },
  {
    "taskId": "task_1721311547692",
    "createdAt": "2024-07-18T14:05:48.000+00:00",
    "id": 2,
    "taskType": 0
  },
  {
    "taskId": "task_1721525155296",
    "createdAt": "2024-07-21T01:25:55.000+00:00",
    "id": 3,
    "taskType": 1
  }
])

const handleclick = (taskId) => {
  console.log("taskid is ", taskId)
  router.push({
    path: '/exerciselog',
    query: {
      task_id: taskId
    }
  })
}

const getTaskList = async () => {
  try {
    const res = await axios.get(`http://${ipAddr}/user/getTasks`)
    if (res.data.status === 200) {
      tabledata.value = res.data.data.tasklist
      for (let i = 0; i < tabledata.value.length; i++) {
        if (tabledata.value[i].taskType == 0) {
          tabledata.value[i].taskType = "自定义任务"
        } else {
          tabledata.value[i].taskType = "基础训练任务"
        }
        tabledata.value[i].createdAt = formatDate(tabledata.value[i].createdAt)
      }
    } else {
      console.log("数据获取失败,检查后端服务是否开启")
    }
  } catch (error) {
    console.log("发生异常", error)
  }
}

const formatDate = (dateString) => {
  const options = { year: 'numeric', month: 'long', day: 'numeric', hour: '2-digit', minute: '2-digit' };
  const date = new Date(dateString);
  return date.toLocaleString('zh-CN', options);
}

getTaskList()
</script>

<template>
  <van-sticky>
    <van-nav-bar title="任务记录" />
  </van-sticky>
  <van-notice-bar color="#1989fa" background="#ecf9ff" left-icon="volume-o" :scrollable="false">
  <van-swipe
    vertical
    class="notice-swipe"
    :autoplay="3000"
    :touchable="false"
    :show-indicators="false"
  >
    <van-swipe-item>点击即可查看每个任务对应的训练记录</van-swipe-item>
    <van-swipe-item>欢迎使用言阻声开智能口吃矫正助手！</van-swipe-item>
    <van-swipe-item>基础训练任务使用我们为您提供的训练文本</van-swipe-item>
  </van-swipe>
</van-notice-bar>


  <van-cell-group v-for="item in tabledata" @click="handleclick(item.taskId)" :key="item.id">
    <van-cell
      :title="`序号：${item.id}`" 
      :value="item.taskType" 
      :label="item.createdAt" />
    <van-divider :style="{ color: '#1989fa', borderColor: '#1989fa', padding: '0 16px' }">
      我是分割线
    </van-divider>
  </van-cell-group>
  <van-skeleton title :row="3" />
</template>

<style scoped>
.notice-swipe {
    height: 40px;
    line-height: 40px;
  }
</style>
