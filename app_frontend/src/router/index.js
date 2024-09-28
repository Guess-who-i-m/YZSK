import { createRouter, createWebHashHistory } from 'vue-router'

const router = createRouter({
  history: createWebHashHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      component: () => import('@/views/choose.vue')
    },
    {
      path: '/user',
      component: () => import('@/views/user.vue')
    },
    {
      path: '/choosemode',
      component: () => import('@/views/choose.vue')
    },
    {
      path: '/record',
      component: () => import('@/views/record.vue')
    },
    {
      path: '/choosetext',
      component: () => import('@/views/choose/choosetext.vue')
    },
    {
      path: '/hadchoose',
      component: () => import('@/views/choose/hadchoose.vue')
    },
    {
      path: '/hadchooses',
      component: () => import('@/views/choose/hadchooses.vue')
    },
    {
      path: '/exerciselog',
      component: () => import('@/views/exercise/exerciselog.vue')
    },
    // {
    //   path: '/test',
    //   component: () => import('@/views/test.vue')
    // },
    {
      path: '/detail',
      component: () => import('@/views/exercise/detail.vue')
    },
    // 添加一个 catch-all 路由来处理未定义的路径
    {
      path: '/:pathMatch(.*)*',
      redirect: '/'
    }
  ]
})

export default router
