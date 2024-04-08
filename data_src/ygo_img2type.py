import cv2
import numpy as np


def get_card_type(card_color):
    if card_color == 'red':
        return '陷阱卡'
    elif card_color == 'green':
        return '魔法卡'
    else:
        return '未知类型'

# 定义颜色范围
color_ranges = {
    'red': ((0, 100, 100), (10, 255, 255)),  # 红色范围
    'green': ((35, 100, 100), (85, 255, 255))  # 绿色范围
}

# 读取游戏王卡片图片
image = cv2.imread('game_card.jpg')

# 将图像转换为HSV颜色空间
hsv_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

# 遍历颜色范围并创建颜色掩码
for color, (lower, upper) in color_ranges.items():
    mask = cv2.inRange(hsv_image, np.array(lower), np.array(upper))
    # 计算颜色区域的像素数量
    pixel_count = cv2.countNonZero(mask)
    # 如果颜色区域的像素数量大于一定阈值，则认为是该颜色的卡片
    if pixel_count > 1000:
        card_type = get_card_type(color)
        print("这张卡片是{}，边框颜色为{}".format(card_type, color))
        # 可以在这里加入其他处理，比如显示图片、保存结果等
        break  # 如果已经确定了卡片类型，则退出循环
else:
    print("未识别到有效的卡片")
