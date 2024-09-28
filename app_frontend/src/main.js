import { createApp } from 'vue'
import { createPinia } from 'pinia'
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'
import ECharts from 'vue-echarts';
import 'echarts'
import Vant from 'vant';
import App from './App.vue'
import router from './router'
import { Notify } from 'vant';
import 'vant/lib/index.css';    
const app = createApp(App).component('ECharts',ECharts);

app.use(createPinia())
app.use(Vant)
app.use(router)
app.use(ElementPlus)
app.use(Notify);
app.mount('#app')
