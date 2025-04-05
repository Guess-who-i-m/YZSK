from rest_framework.decorators import api_view
from rest_framework.response import Response
from django.http import JsonResponse
from django.shortcuts import get_object_or_404
from webapi.models import Session, Log, Typestat, Heartrateoxygen, Audio, Image, Text, Speechtext, Now, Task
from django.db.models import Avg
from datetime import datetime

@api_view(["GET"])
def findAll(request):
    # 对应 userService.searchAllUser()
    # 这里假设你有一个 User 模型或可替换当前逻辑
    # user_list = list(User.objects.values())  # 如使用内置 Django User
    user_list = [
        {"userId": 1, "name": "Alice"},
        {"userId": 2, "name": "Bob"}
    ]
    return Response({"data": user_list})

@api_view(["GET"])
def getScores(request):
    # 对应 userService.getScoreCharts()
    # 这里假设直接获取所有 Session 并返回其中的 score 字段
    sessions = list(Session.objects.values("session_id", "score"))
    return Response({"data": sessions})

@api_view(["GET"])
def getBasicText(request):
    # 对应 userService.generateBasicTexts()
    # 获取所有基础训练文本 Text
    texts = list(Text.objects.filter(rank__gt=0).values("text_id", "text_content", "rank"))
    return Response({"data": texts})

@api_view(["POST"])
def uploadSpeechText(request):
    # 对应 userService.processSpeechText(speechtext)
    # 前端通过参数 speechtext 上传
    speechtext = request.data.get("speechtext")
    if not speechtext:
        return Response({"error": "the speech text is null"}, status=400)
    Speechtext.objects.create(text=speechtext, rank=999)  # 示例逻辑
    return Response({"message": "upload speechtext successfully!"})

@api_view(["GET"])
def getSpeechText(request):
    # 对应 userService.generateSpeechTexts()
    # 获取所有演讲文本 Speechtext
    speech_list = list(Speechtext.objects.values("speechtext_id", "text", "rank"))
    return Response({"data": speech_list})

@api_view(["GET"])
def getImagesById(request, id):
    # 对应 userService.getImageById(id)
    # 返回 id 对应的图片
    images = list(Image.objects.filter(image_id=id).values())
    if not images:
        return Response({"error": "there is no image for this id!"}, status=404)
    return Response({"data": images})

@api_view(["GET"])
def getDetails(request, id):
    # 对应 userService.generateDetail(id)
    # 此处仅示例查询，实际可返回更多细节
    detail = {"id": id, "description": f"Detail info for {id}"}
    return Response({"data": detail})

@api_view(["GET"])
def getTasks(request):
    # 对应 userService.getAllTasks()
    tasks = list(Task.objects.values("task_id", "task_type"))
    if tasks:
        return Response({"data": tasks})
    return Response({"error": "get tasks failed!"}, status=400)

@api_view(["GET"])
def getSessionMsg(request, task_id):
    # 对应 userService.getSessionByTask(task_id) + 根据 session 获取数据
    sessions = Session.objects.filter(task_id=task_id)
    if not sessions.exists():
        return Response({"error": "there is no data for this task!"}, status=400)
    data = {}
    # 根据Session获取对应图片和心率血氧信息
    list_of_images = []
    list_of_heartrateoxygens = []
    for s in sessions:
        ims = list(Image.objects.filter(session=s).values())
        hos = list(Heartrateoxygen.objects.filter(session=s).values())
        list_of_images.append(ims)
        list_of_heartrateoxygens.append(hos)
    data["listofimages"] = list_of_images
    data["listofheartrateoxygens"] = list_of_heartrateoxygens
    return Response({"data": data})

@api_view(["GET"])
def getSessionLogs(request, task_id):
    # 对应 userService.getSessionsByTaskId(task_id)
    session_list = list(Session.objects.filter(task_id=task_id).values())
    if not session_list:
        return Response({"error": "there is no data for this task!"})
    return Response({"data": session_list})

@api_view(["GET"])
def getTypeStat(request, task_id):
    # 对应 userService.calculateSumBySessionIds(sessionIds)
    sessions = Session.objects.filter(task_id=task_id)
    if not sessions.exists():
        return Response({"error": "task_id is invalid or not found"})
    session_ids = sessions.values_list("session_id", flat=True)
    # 聚合统计 Typestat
    total_stats = {
        "interrupt": 0,
        "smooth": 0,
        "toolong": 0,
        "repeatA": 0,
        "repeatB": 0,
        "repeatC": 0
    }
    stats = Typestat.objects.filter(session__session_id__in=session_ids)
    for stat in stats:
        total_stats["interrupt"] += stat.interrupt or 0
        total_stats["smooth"] += stat.smooth or 0
        total_stats["toolong"] += stat.toolong or 0
        total_stats["repeatA"] += stat.repeatA or 0
        total_stats["repeatB"] += stat.repeatB or 0
        total_stats["repeatC"] += stat.repeatC or 0
    return Response({"data": total_stats})

@api_view(["POST"])
def buildNewTask(request, rank):
    # 对应 userService.setTaksMsg(rank) + userService.setNowRank(rank)
    # 这里仅示例创建一个 Task 并返回
    if rank is None:
        return Response({"error": "rank参数不能为空"}, status=400)
    new_task_id = f"task_{datetime.now().strftime('%Y%m%d%H%M%S')}"
    Task.objects.create(task_id=new_task_id, task_type=(1 if rank != 0 else 0))
    return Response({"message": "set a task successfully!", "task_id": new_task_id})

@api_view(["GET"])
def gethoOfSession(request, sessionId):
    # 对应 userService.getHeartrateoxygen(sessionId) + userService.getTypestatBySessionId(sessionId)
    hos = list(Heartrateoxygen.objects.filter(session__session_id=sessionId).values())
    typestat = Typestat.objects.filter(session__session_id=sessionId).first()
    return Response({
        "heartrateoxygens": hos,
        "typestat": {
            "interrupt": typestat.interrupt if typestat else 0,
            "smooth": typestat.smooth if typestat else 0,
            "toolong": typestat.toolong if typestat else 0,
            "repeatA": typestat.repeatA if typestat else 0,
            "repeatB": typestat.repeatB if typestat else 0,
            "repeatC": typestat.repeatC if typestat else 0
        }
    })

@api_view(["GET"])
def getGptAdvice(request):
    # 对应 Txqw.callWithMessage(); 这里只给出示例
    advice = "This is GPT's suggestion based on your data."
    return Response({"message": advice})

@api_view(["GET"])
def getHodataBytaskid(request, task_id):
    # 对应 userService.getSessionsByTaskId(task_id) 然后计算平均值
    sessions = Session.objects.filter(task_id=task_id)
    averages = []
    for s in sessions:
        hos = Heartrateoxygen.objects.filter(session=s)
        if hos.exists():
            heart_avg = hos.aggregate(Avg("heart_rate"))["heart_rate__avg"]
            oxygen_avg = hos.aggregate(Avg("oxygen_level"))["oxygen_level__avg"]
            averages.append({"session_id": s.session_id, "heartRateAverage": heart_avg, "oxygenLevelAverage": oxygen_avg})
        else:
            averages.append({"session_id": s.session_id, "heartRateAverage": 0, "oxygenLevelAverage": 0})
    return Response({"data": averages})

@api_view(["GET"])
def getImageBySessionId(request, sessionId):
    # 对应 userService.getImageBySession(sessionId)
    imagelist = list(Image.objects.filter(session__session_id=sessionId).values())
    return Response({"data": imagelist})
