<script setup>
import{onMounted, ref} from 'vue'
import axios from 'axios';
import { useRoute,useRouter } from 'vue-router';
import ipAddr from '@/api/index';
import { showSuccessToast, showFailToast } from 'vant';
//
const router=useRouter()
const id=ref(0)
const issetup=ref(0)
// const route=useRoute()
// const newtext=ref('')
// newtext.value=route.query.text
// console.log(newtext)
//函数定义
const goback=()=>{
    router.push('/choosemode')
}
//
// showSuccessToast('创建任务成功！');
const setPraText=async()=>{
    console.log('id is   ',id.value)
    // id.value=id.value-1
    const res=await axios.post(`http://${ipAddr}/user/buildNewTask/${id.value}`)
    .then((res)=>{
        if(res.data.status==200){
        issetup.value=1
        showSuccessToast('创建任务成功！');
    }else{
        showFailToast('创建任务失败');
    }
    }).catch((error)=>{
        showFailToast("创建任务失败")
        console.log(error)
    })
}
//
setPraText()
</script>

<template>
       <van-nav-bar
  title="任务创建"
  left-text="返回"
  left-arrow
  @click-left="goback()"
/>
<div v-if="issetup">
    <div style="padding: 10px;">
        您已经成功创建好一个任务，现在可以使用开发板进行练习！
    </div>
    <div style="padding: 10px;" >
        您现在可以自由选择文本进行训练
    </div>
</div>
<div v-else>
    <div style="padding: 10px;">
        创建任务失败，检查您的网络连接是否正常
    </div>
</div>
</template>