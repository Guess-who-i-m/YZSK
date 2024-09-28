<template>
    <div class="markdown-body" v-html="compiledMarkdown"></div>
  </template>
  
  <script setup>
  import { ref, watch, toRef } from 'vue';
  import { marked } from 'marked';
  import hljs from 'highlight.js';
  import 'highlight.js/styles/github.css';
  import 'github-markdown-css/github-markdown.css'; // 引入 GitHub 风格的 Markdown 样式
  
  const props = defineProps({
    content: {
      type: String,
      required: true
    }
  });
  
  const compiledMarkdown = ref('');
  
  watch(toRef(props, 'content'), (newMarkdown) => {
    compiledMarkdown.value = marked(newMarkdown, {
      highlight: (code, lang) => {
        return hljs.highlightAuto(code, [lang]).value;
      }
    });
  });
  
  // 初始渲染
  compiledMarkdown.value = marked(props.content, {
    highlight: (code, lang) => {
      return hljs.highlightAuto(code, [lang]).value;
    }
  });
  </script>
  
  <style scoped>
  .markdown-body {
    padding: 30px;
  background-color: #ffffff; /* 背景颜色为白色 */
  border-radius: 10px;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.1);
  line-height: 1.8;
  margin-top: 20px;
  font-size: 18px;
  color: #227d99; /* 字体颜色为浅蓝色 */
  }
  </style>
  