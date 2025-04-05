from django.db import models

class Session(models.Model):
    session_id = models.CharField(max_length=64, primary_key=True)
    task_id = models.CharField(max_length=64, blank=True, null=True)
    score = models.IntegerField(null=True, blank=True)
    created_at = models.DateTimeField(auto_now_add=True)
    # ...other fields...

class Log(models.Model):
    log_id = models.AutoField(primary_key=True)
    session = models.ForeignKey(Session, on_delete=models.CASCADE)
    user_id = models.IntegerField()
    text_id = models.IntegerField()
    text_type = models.IntegerField()
    # ...other fields...

class Typestat(models.Model):
    typestat_id = models.AutoField(primary_key=True)
    session = models.ForeignKey(Session, on_delete=models.CASCADE)
    interrupt = models.IntegerField(null=True, blank=True)
    smooth = models.IntegerField(null=True, blank=True)
    toolong = models.IntegerField(null=True, blank=True)
    repeatA = models.IntegerField(null=True, blank=True)
    repeatB = models.IntegerField(null=True, blank=True)
    repeatC = models.IntegerField(null=True, blank=True)
    # ...other fields...

class Heartrateoxygen(models.Model):
    ho_id = models.AutoField(primary_key=True)
    session = models.ForeignKey(Session, on_delete=models.CASCADE)
    heart_rate = models.IntegerField()
    oxygen_level = models.FloatField()
    record_time = models.DateTimeField(auto_now_add=True)
    # ...other fields...

class Audio(models.Model):
    audio_id = models.AutoField(primary_key=True)
    session = models.ForeignKey(Session, on_delete=models.CASCADE)
    audio_url = models.CharField(max_length=255)
    # ...other fields...

class Image(models.Model):
    image_id = models.AutoField(primary_key=True)
    session = models.ForeignKey(Session, on_delete=models.CASCADE)
    image_data = models.TextField()
    image_url = models.CharField(max_length=255, blank=True, null=True)
    # ...other fields...

class Text(models.Model):
    text_id = models.AutoField(primary_key=True)
    text_content = models.TextField()
    rank = models.IntegerField()
    # ...other fields...

class Speechtext(models.Model):
    speechtext_id = models.AutoField(primary_key=True)
    text = models.TextField()
    rank = models.IntegerField()
    # ...other fields...

class Now(models.Model):
    now_id = models.AutoField(primary_key=True)
    session_now = models.CharField(max_length=64)
    # ...other fields...

class Task(models.Model):
    task_id = models.CharField(max_length=64, primary_key=True)
    task_type = models.IntegerField()
    # ...other fields...
