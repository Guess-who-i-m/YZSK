"""
URL configuration for django_back_end project.

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/5.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path
from webapi.views import (
    upload_audio,
    upload_image,
    training_text,
    upload_heartrate_oxygen,
    get_prac_begin,
    get_the_text,
    get_now_score,
)

urlpatterns = [
    path('admin/', admin.site.urls),
    path('uploadAudio/', upload_audio, name='upload_audio'),
    path('uploadImage/', upload_image, name='upload_image'),
    path('training/text/<int:rank>/', training_text, name='training_text'),
    path('uploadHeartrateOxygen/', upload_heartrate_oxygen, name='upload_hr_oxy'),
    path('upload/getPracBegin/', get_prac_begin, name='get_prac_begin'),
    path('getTheText/', get_the_text, name='get_the_text'),
    path('getNowScore/', get_now_score, name='get_now_score'),
]
