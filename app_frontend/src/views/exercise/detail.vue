<script setup>
import { ref, onMounted, computed } from 'vue'
import { useRoute, useRouter } from 'vue-router';
import ipAddr from '@/api/index';
import { Notify } from 'vant';
import * as echarts from 'echarts';
import axios from 'axios';
import ECharts from 'vue-echarts'

const route = useRoute()
const router = useRouter()
const sessionId = ref('')
const taskId = ref('')
const score = ref(0)
score.value = route.query.score
taskId.value = route.query.task_id
sessionId.value = route.query.sessionId
console.log(sessionId.value)
console.log(taskId.value)

const typestats = ref({
  "id": 3,
  "smooth": 1,
  "interrupt": 1,
  "repeatA": 2,
  "repeatB": 0,
  "repeatC": 1,
  "toolong": 0,
  "sessionId": "sess_1720925388727"
})


const imageList=ref([
{
        "id": 163,
        "sessionId": "sess_1720925388727",
        "imageUrl": "http://yanzushengkai.oss-cn-beijing.aliyuncs.com/6b0c41152d274cbf83d5e97e01a8445a.png",
        "imageData": "Neutral"
      },
      {
        "id": 164,
        "sessionId": "sess_1720925388727",
        "imageUrl": "http://yanzushengkai.oss-cn-beijing.aliyuncs.com/ea57b112c5294c18952c424908610eb1.png",
        "imageData": "Happy"
      }
])

const hodata = ref([
  {
    "id": 14,
    "sessionId": "sess_1720925388727",
    "heartRate": 72,
    "oxygenLevel": 98.5,
    "recordTime": "2024-07-23T19:25:22.000+00:00"
  },
  {
    "id": 15,
    "sessionId": "sess_1720925388727",
    "heartRate": 73,
    "oxygenLevel": 98.6,
    "recordTime": "2024-07-23T19:25:22.000+00:00"
  },
])

// 函数定义
const goback = () => {
  router.push(`/exerciselog?task_id=${taskId.value}`)
}

const getDataList = async (sessionId) => {
  console.log("打印出对象", typestats.value)
  try {
    const res = await axios.get(`http://${ipAddr}/user/getheartoxygen/${sessionId}`)
    console.log(res)
    if (res.data.status === 200) {
      typestats.value = res.data.data.typestat
      hodata.value = res.data.data.heartrateoxygens// 假设后台返回的心率和氧气数据是 hodata
    } else {
      console.log(res.data.message)
    }
  } catch (error) {
    console.log("发生异常" + error)
  }
}

const getImageData=async (sessionId) => {
  console.log("打印出对象", typestats.value)
  try {
    const res = await axios.get(`http://${ipAddr}/user/getImageBySessionId/${sessionId}`)
    console.log(res)
    if (res.data.status === 200) {
      imageList.value = res.data.data.imagelist
      // hodata.value = res.data.data.heartrateoxygens// 假设后台返回的心率和氧气数据是 hodata
    } else {
      console.log(res.data.message)
    }
  } catch (error) {
    console.log("发生异常" + error)
  }
}

//函数调用
onMounted(() => {
  getDataList(sessionId.value)
  getImageData(sessionId.value)
})

// ECharts 柱状图选项
const optionBar = computed(() => ({
  title: {
    text: '本次练习口吃问题识别及统计',
    left: 'center',
    textStyle: {
      fontSize: 14 // 调整标题字体大小
    }
  },
  backgroundColor: '#fff',
  tooltip: {
    trigger: 'axis',
    axisPointer: {
      type: 'shadow'
    }
  },
  grid: {
    left: '3%',
    right: '4%',
    bottom: '3%',
    containLabel: true
  },
  xAxis: {
    type: 'category',
    data: ['流畅', '间断', '音节重复', '字重复', '词重复', '过分长音'],
    axisTick: {
      alignWithLabel: true
    }
  },
  yAxis: {
    type: 'value'
  },
  series: [
    {
      name: '次数',
      type: 'bar',
      barWidth: '60%',
      data: [
        typestats.value.smooth,
        typestats.value.interrupt,
        typestats.value.repeatA,
        typestats.value.repeatB,
        typestats.value.repeatC,
        typestats.value.toolong
      ]
    }
  ]
}));

// ECharts 折线图选项
const optionLine = computed(() => ({
  title: {
    text: '心率和血氧浓度随时间变化',
    left: 'center',
    textStyle: {
      fontSize: 14 // 调整标题字体大小
    }
  },
  backgroundColor: '#fff',
  tooltip: {
    trigger: 'axis',
    axisPointer: {
      type: 'line'
    }
  },
  legend: {
    data: ['心率', '血氧浓度(%)'],
    bottom: '0'
  },
  grid: {
    left: '3%',
    right: '4%',
    bottom: '10%',
    containLabel: true
  },
  xAxis: {
    type: 'category',
    data: hodata.value.map(item => new Date(item.recordTime).toLocaleString('zh-CN', { hour: '2-digit', minute: '2-digit' })),
    axisTick: {
      alignWithLabel: true
    }
  },
  yAxis: {
    type: 'value'
  },
  series: [
    {
      name: '心率',
      type: 'line',
      data: hodata.value.map(item => item.heartRate)
    },
    {
      name: '血氧浓度(%)',
      type: 'line',
      data: hodata.value.map(item => item.oxygenLevel)
    }
  ]
}));

// ECharts ImageData 折线图选项
const optionImageLine = computed(() => ({
  title: {
    text: '图片情绪数据',
    left: 'center',
    textStyle: {
      fontSize: 14 // 调整标题字体大小
    }
  },
  backgroundColor: '#fff',
  tooltip: {
    trigger: 'axis',
    axisPointer: {
      type: 'line'
    }
  },
  legend: {
    data: ['情绪数据'],
    bottom: '0'
  },
  grid: {
    left: '3%',
    right: '4%',
    bottom: '10%',
    containLabel: true
  },
  xAxis: {
    type: 'category',
    data: imageList.value.map(item => item.id),
    axisTick: {
      alignWithLabel: true
    }
  },
  yAxis: {
    type: 'category',
    data: ['Angry', 'Disgust', 'Fear', 'Happy', 'Sad', 'Surprise', 'Neutral']
  },
  series: [
    {
      name: '情绪数据',
      type: 'line',
      data: imageList.value.map(item => ({
        value: item.imageData,
        label: {
          show: true,
          position: 'top'
        }
      }))
    }
  ]
}));

</script>

<template>
  <van-sticky>
    <van-nav-bar
      title="训练详情"
      left-text="返回"
      left-arrow
      @click-left="goback"
    />
  </van-sticky>
  <van-cell :title="`${score}`" size="large" label="本次得分" />
  <div class="chart">
    <ECharts :option="optionBar" auto-resize style="width: 100%; height: 400px;" />
  </div>
  <div class="chart">
    <ECharts :option="optionLine" auto-resize style="width: 100%; height: 400px;" />
  </div>
  <div class="chart">
    <ECharts :option="optionImageLine" auto-resize style="width: 100%; height: 400px;" />
  </div>
  <van-skeleton title :row="3" />
</template>

<style scoped>
.chart {
  margin: 10px 0;
}
</style>
