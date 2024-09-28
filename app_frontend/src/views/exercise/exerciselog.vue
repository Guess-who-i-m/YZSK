<script setup>
import { ref, onMounted, computed } from 'vue'
import { useRoute, useRouter } from 'vue-router';
import MarkdownIt  from 'markdown-it';
import ipAddr from '@/api/index';
import 'echarts'
import { Notify } from 'vant';
import * as echarts from 'echarts';
import axios from 'axios';
import ECharts from 'vue-echarts'

import MarkdownRenderer from '@/components/markdown.vue';

// import { makeArrayProp } from 'vant/lib/utils';
// import { da } from 'element-plus/es/locale';
const JiaZai=ref(0)
const first=ref(0)
const route = useRoute()
const router = useRouter()
const taskId = ref('')
taskId.value = route.query.task_id
console.log(taskId.value)
const isHave = ref(0)
const advice=ref('')
const tableData = ref([
{
    "id": 46,
    "sessionId": "sess_1721838208576",
    "createdAt": "2024-07-24T16:23:29.000+00:00",
    "taskId": "task_1721838185927",
    "score": 11
      }
])
const askAdviceData=ref(
  {
  "smooth": 10,
  "interrupt": 2,
  "repeatA": 3,
  "repeatB": 1,
  "repeatC": 0,
  "toolong": 1,
  "readings": [
    {
      "heartRateAverage": 73,
      "oxygenLevelAverage": 98.6
    },
    {
      "heartRateAverage": 75,
      "oxygenLevelAverage": 97.8
    }
  ]
}
)
const AverageHoData=ref([
{
        "heartRateAverage": 0,
        "oxygenLevelAverage": 0
      },
      {
        "heartRateAverage": 84.42857142857143,
        "oxygenLevelAverage": 95.4
      },
      {
        "heartRateAverage": 0,
        "oxygenLevelAverage": 0
      }
])
//echarts


const goback = () => {
  router.push('/record')
}

const handleclick=(index)=>{
  console.log("index is ",index)
  console.log("tableData is ",tableData.value)
    router.push({
        path:'/detail',
        query:{
            sessionId:tableData.value[index-1].sessionId,
            task_id:taskId.value,
            score:tableData.value[index-1].score
        }
    })
}

const handleAdvice=()=>{
      JiaZai.value=1
  //填充数据
      askAdviceData.value.smooth=data.value[0].value
      askAdviceData.value.interrupt=data.value[1].value
      askAdviceData.value.repeatA=data.value[2].value
      askAdviceData.value.repeatB=data.value[3].value
      askAdviceData.value.repeatC=data.value[4].value
      askAdviceData.value.toolong=data.value[5].value
    
      askAdviceData.value.readings=AverageHoData.value
      console.log("待发送的数据",askAdviceData.value)
      getQwenAdvice()
    //有待考虑
}

const getSessionList = async (taskId) => {
  console.log(taskId)
  try {
    const res = await axios.get(`http://${ipAddr}/user/SessionLogs/${taskId}`)
    if (res.data.status === 200) {
      tableData.value = res.data.data.sessionlist
      first.value=tableData.value[0].id-1
      console.log('最开始的值是：',first.value)
      for (let i = 0; i < tableData.value.length; i++) {
        tableData.value[i].id = i + 1
        tableData.value[i].createdAt = formatDate(tableData.value[i].createdAt)
      }
      isHave.value = 1
    } else {
      isHave.value = 0
      console.log(res)
      console.log(res.data.message)
      tableData.value = null
    }
  } catch (error) {
    console.log("发生异常" + error)
    tableData.value = null
  }
}

const getTypestatList = async (taskId) => {
  console.log(taskId)
  try {
    const res = await axios.get(`http://${ipAddr}/user/Typestat/${taskId}`)
    // console.log('************'+res)
    if (res.data.status === 200) {
      data.value[0].value = res.data.data.totalData.smooth
      data.value[1].value=res.data.data.totalData.interrupt
      data.value[2].value=res.data.data.totalData.repeatA
      data.value[3].value=res.data.data.totalData.repeatB
      data.value[4].value=res.data.data.totalData.repeatC
      data.value[5].value=res.data.data.totalData.toolong
      console.log("data is ",data.value)
      // 获取第一个元素的 id
    }else
    {
      // isHave=0
      console.log(res)
    //   console.log(res.data.message)
    }
  } catch (error) {
    // console.log(res)
    
    console.log("发生异常")
    data.value=null
  }
}

const getAverageHoData =async()=>{
  console.log("平均",taskId.value)
    const res=await axios.get(`http://${ipAddr}/user/getHodataBytaskid/${taskId.value}`)
    .then((res)=>{
      if(res.data.status==200){
          AverageHoData.value=res.data.data.averages
          console.log("成功获取心率血氧平均数据")
          console.log("平均",AverageHoData.value)
      }else
      {
        console.log(res.data.message)
      }
    }).catch((error)=>[
      console.log("获取平均心率血氧数据"+error)
    ])
}

const getQwenAdvice=async ()=>{
  try {
    const res = await axios.post(`http://${ipAddr}/chat`, askAdviceData.value, {
      headers: {
        'Content-Type': 'application/json'
      }
    });
    // 处理响应数据
    console.log('响应数据:', res.data);
    advice.value=res.data.output.text
    markdownContent.value=advice.value
    JiaZai.value=0
  } catch (error) {
    // 处理错误
    console.error('请求出错:', error);
  }
}


const formatDate = (dateString) => {
  const options = { year: 'numeric', month: '2-digit', day: '2-digit', hour: '2-digit', minute: '2-digit' };
  const date = new Date(dateString);
  return date.toLocaleString('zh-CN', options);
}

onMounted(() => {
  getSessionList(taskId.value)
  getTypestatList(taskId.value)
  getAverageHoData(taskId.value)
  if (isHave.value == 0) {
    console.log('无')
  } else {
    console.log('成功')
  }
})


const data = ref([
  { value: 1, name: '流畅' },
  { value: 2, name: '间断' },
  { value: 3, name: '音节重复' },
  { value: 4, name: '字重复' },
  { value: 5, name: '词重复' },
  { value: 6, name: '过分长音' },
]);
const option1 = computed(() => {
  return {
    tooltip: {
      trigger: 'axis',
      axisPointer: {
        type: 'cross'
      },
      formatter: function (params) {
        const param = params[0];
        return `Date: ${param.axisValueLabel}<br>Score: ${param.data}`;
      }
    },
    xAxis: {
      type: 'category',
      axisLine: {
        symbol: 'arrow',
        lineStyle: {
          type: 'dashed'
        }
      },
      data: tableData.value.map(d => d.createdAt),
    },
    yAxis: {
      type: 'value',
      axisLine: {
        symbol: 'arrow',
        lineStyle: {
          type: 'dashed'
        }
      }
    },
    title: {
      text: '训练-得分图',
      left: 'center',
      textStyle: {
        fontSize: 14 // 调整标题字体大小
      }
    },
    series: [
      {
        data: tableData.value.map(d => d.score),
        type: 'line',
      }
    ]
  }
})

const option2 = computed(() => ({
  title: {
    text: '口吃问题识别及统计',
    left: 'center',
    textStyle: {
      fontSize: 14 // 调整标题字体大小
    }
  },
  tooltip: {
    trigger: 'item',
    formatter: '{a} <br/>{b} : {c} ({d}%)'
  },
  legend: {
    left: 'center',
    top: 'bottom',
    data: data.value.map(item => item.name), // 更新这里使用动态数据生成图例项
    textStyle: {
      fontSize: 10 // 调整图例字体大小
    }
  },
  toolbox: {
    show: true,
    feature: {
      mark: { show: true },
      dataView: { show: true, readOnly: false },
      restore: { show: true },
      saveAsImage: { show: true }
    }
  },
  series: [
    {
      name: 'Area Mode',
      type: 'pie',
      radius: [20, 100], // 调整饼图的大小
      roseType: 'area',
      itemStyle: {
        borderRadius: 5
      },
      data: data.value.map(item => ({
        value: item.value,
        name: item.name
      }))
    }
  ]
}));


const markdownContent = ref(``);
</script>

<template>
    <van-sticky>
  <van-nav-bar
    title="训练记录"
    left-text="返回"
    left-arrow
    @click-left="goback()"
  />
</van-sticky>
  <div class="chart">
    <ECharts :option="option1" auto-resize style="width: 100%; height: 350px;" />
  </div>
  <div class="chart">
    <ECharts :option="option2" auto-resize style="width: 100%; height: 500px;" />
  </div>
  <van-cell title="训练记录一览"  value="点击以查看详情" size="large" />
  <van-cell-group>
    <van-cell 
      v-for="item in tableData" 
      :key="item.id"
      :title="`序号：${item.id}`" 
      :value="`得分：${item.score}`" 
      :label="`${item.createdAt}`" 
      @click="handleclick(item.id)"
    />
  </van-cell-group>
  <van-button type="primary" block @click="handleAdvice()">AI生成建议</van-button>
  <!-- <div v-if="advice" class="advice">{{ advice }}</div> -->
    <div style="padding: 10px;">
  <van-loading size="24px" v-if="JiaZai">正在为您努力生成中......</van-loading>
    </div>
  <div>
    <MarkdownRenderer :content="markdownContent" />
  </div>
  <van-skeleton title :row="3" />
</template>

<style scoped>
.chart {
  width: 100%;
  margin: 10px 0;
}
.advice {
  margin-top: 20px;
  padding: 10px;
  background-color: #f0f0f0;
  border: 1px solid #ccc;
  border-radius: 5px;
}
</style>
