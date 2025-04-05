import requests
import random
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt

def clean_markdown(text):
    text = text.replace('#', '')
    text = text.replace('*', '')
    return text.strip()

@csrf_exempt
def chat(request):
    if request.method == 'POST':
        api_url = "your_api_url_here"  # 对应 @Value("${api.url}")
        api_key = "your_api_key_here"  # 对应 @Value("${api.key}")

        q = "some processed string"

        payload = {
            "model": "qwen-turbo",
            "input": {
                "messages": [
                    {"role": "system", "content": "你是一名资深的口吃矫正专家"},
                    {"role": "user", "content": q}
                ]
            },
            "parameters": {"result_format": "text"}
        }
        try:
            response = requests.post(
                api_url,
                headers={"Authorization": f"Bearer {api_key}",
                         "Content-Type": "application/json"},
                json=payload
            )
            data = response.json()
        except Exception as e:
            return JsonResponse({"result": None, "extra": None, "error": f"Request failed: {str(e)}"})
        return JsonResponse(data)

@csrf_exempt
def get_now_advice(request):
    if request.method == 'GET':
        api_url = "your_api_url_here"
        api_key = "your_api_key_here"

        q_list = [
            "我在进行言语训练时无法流畅读出训练内容...",
            "我在进行言语训练时可以流畅的读出一小部分内容...",
            "我在进行言语训练时很难完成整个训练..."
        ]
        selected_question = random.choice(q_list)

        payload = {
            "model": "qwen-turbo",
            "input": {
                "messages": [
                    {"role": "system", "content": "你是一名资深的口吃矫正专家"},
                    {"role": "user", "content": selected_question}
                ]
            },
            "parameters": {"result_format": "text"}
        }
        try:
            response = requests.post(
                api_url,
                headers={"Authorization": f"Bearer {api_key}",
                         "Content-Type": "application/json"},
                json=payload
            )
            result_text = response.text
            cleaned_text = clean_markdown(result_text)
            data = {}  # 视需求封装返回的 JSON
        except Exception as e:
            return JsonResponse({"result": None, "extra": None, "error": f"Request failed: {str(e)}"})
        return JsonResponse({"result": cleaned_text})
