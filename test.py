import requests
import json

# 定义要发送的 JSON 数据
data_to_send = {"data": "Hello World"}

# 服务器地址
url = "http://192.168.137.214"  # 替换为 ESP32 的 IP 地址

# 将数据转换为 JSON 格式并发送 POST 请求
response = requests.post(url, json=data_to_send)

# 打印响应内容
print("Response status code:", response.status_code)
print("Response body:", response.text)
