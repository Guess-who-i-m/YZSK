from rest_framework.decorators import api_view
from rest_framework.response import Response
from django.db.models import F
from django.http import JsonResponse
from django.core.files.base import ContentFile
from base64 import b64decode
from ..models import Session, Typestat, Heartrateoxygen, Image, Now, Task
import random, os

@api_view(['POST'])
def upload_audio(request):
    session_obj = Now.objects.first()
    if not session_obj:
        return Response({"error": "no session found"}, status=400)
    session_id = session_obj.session_now
    try:
        session_item = Session.objects.get(session_id=session_id)
    except Session.DoesNotExist:
        return Response({"error": "session not exist"}, status=400)
    file = request.FILES.get('file')
    if not file:
        return Response({"error": "file can not be empty"}, status=400)
    task_qs = Task.objects.filter(task_id=session_item.task_id)
    if task_qs.exists():
        task_type = task_qs.first().task_type
        if task_type == 1:
            score_value = random.randint(70, 80)
        else:
            score_value = random.randint(50, 65)
        session_item.score = score_value
        session_item.save()
    return Response({"message": "upload audio successfully", "audioUrl": "mock/audio/path"})


@api_view(['POST'])
def upload_image(request):
    session_obj = Now.objects.first()
    if not session_obj:
        return Response({"error": "no session found"}, status=400)
    session_id = session_obj.session_now
    try:
        Session.objects.get(session_id=session_id)
    except Session.DoesNotExist:
        return Response({"error": "the session is not exist"}, status=400)
    image_data = request.body
    if not image_data:
        return Response({"error": "invalid image data"}, status=400)
    new_image = Image(session_id=session_id)
    new_image.image_data = "base64-content"
    new_image.save()
    return Response({"message": "image uploaded successfully"})


@api_view(['POST'])
def upload_heartrate_oxygen(request):
    session_obj = Now.objects.first()
    if not session_obj:
        return Response({"error": "no session found"}, status=400)
    session_id = session_obj.session_now
    try:
        Session.objects.get(session_id=session_id)
    except Session.DoesNotExist:
        return Response({"error": "session not exist"}, status=400)
    data = request.data
    heart_rates = data.get('heart_rate', [])
    spo2_levels = data.get('spo2', [])
    for hr, spo2 in zip(heart_rates, spo2_levels):
        Heartrateoxygen.objects.create(session_id=session_id, heart_rate=hr, oxygen_level=spo2)
    return Response({"message": "upload heartrate successfully"})


@api_view(['GET'])
def get_prac_begin(request):
    new_session = Session.objects.create(session_id="session_mock_"+str(random.randint(1000,9999)))
    Now.objects.update_or_create(now_id=1, defaults={"session_now": new_session.session_id})
    return Response({"message": "get session_id successfully", "session_id": new_session.session_id})


@api_view(['GET'])
def get_the_text(request):
    text_now = "Sample text from the system"
    if not text_now:
        return JsonResponse({"text_now":"error!您选择的是自由模式，没有设置文本"})
    return JsonResponse({
        "status": 200,
        "message": "get text successfully",
        "data": {"text_now": text_now}
    })


@api_view(['GET'])
def get_now_score(request):
    fake_score = random.randint(40, 70)
    return Response({"message":"get now score successfully", "score": fake_score})
