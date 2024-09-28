<script setup>
import{ref} from 'vue'
import axios from 'axios';
import { useRoute,useRouter } from 'vue-router';
import ipAddr from '@/api/index';
import { showSuccessToast, showFailToast } from 'vant';
//
const router=useRouter()
const route=useRoute()
const newtext=ref('')
const id=ref(0)
newtext.value=route.query.text
id.value=route.query.id

//函数定义
const goback=()=>{
    router.push('/choosetext')
}
const setPraText=async()=>{
    console.log('id is   ',id.value)
    id.value=id.value-1
    const res=await axios.post(`http://${ipAddr}/user/buildNewTask/${id.value}`)
    .then((res)=>{
        if(res.data.status==200){
        showSuccessToast('创建任务成功！');
    }else{
        showFailToast('创建任务失败');
    }
    }).catch((error)=>{
        showFailToast("创建任务失败")
        console.log(error)
    })
}
//函数调用
setPraText()

console.log(id.value)
console.log(newtext)
</script>

<template>
       <van-nav-bar
  title="任务创建"
  left-text="返回"
  left-arrow
  @click-left="goback()"
/>
<div style="padding: 10px;">
    您已经成功创建好一个任务，现在可以使用开发板进行练习！
</div>
<div style="padding: 10px;" >
    您的训练文本是：{{ newtext }}
</div>

</template>