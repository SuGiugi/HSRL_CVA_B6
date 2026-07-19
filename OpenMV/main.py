import sensor, image, time
from matrix_mini import send_data, uart, green_led
import fill_light

# Khởi tạo cảm biến
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)

sensor.set_vflip(True)
sensor.set_hmirror(True)

fill_light.on()

threshold_obj = [(37, 18, -128, -15, 127, -128), (19, 9, 19, 127, -128, 67), (36, 5, 28, 127, -128, 31)]
clock = time.clock()
nearest_data = (-1, 0, 0, 0)
def finding(color_id):
    global nearest_data
    if color_id == 2:
        blobs = img.find_blobs([threshold_obj[color_id]], pixels_threshold=700, area_threshold=200)
    else:
        blobs = img.find_blobs([threshold_obj[color_id]], pixels_threshold=600, area_threshold=200)
    if color_id == 2:
        color_id = 1
    if blobs:
        max_blob = max(blobs, key=lambda b: b.cy())
        img.draw_rectangle(max_blob.rect())
        x_center = max_blob.cx()
        y_center = max_blob.cy()
        blob_area = round(max_blob.area() / 2)

        if nearest_data[2] <= y_center:
            nearest_data = (color_id, x_center, y_center, blob_area)

        img.draw_cross(x_center, y_center)
        img.draw_string(max_blob.x(), max_blob.y() - 15, f"X:{x_center}, Y:{y_center}", color=(255, 255, 255))


# while True:
#     fill_light.brightness(100)
#     # Kiểm tra xem robot có gửi lệnh yêu cầu lấy dữ liệu không
#     clock.tick()

#     # CHỤP ẢNH NGAY TẠI THỜI ĐIỂM ROBOT YÊU CẦU
#     img = sensor.snapshot()

#     nearest_data = (-1, 0, 0, 0)
#     finding(0)
#     finding(1)
#     finding(2)
#     # finding(3)

#     # Gửi dữ liệu mới nhất về ngay lập tức
#     if nearest_data != (-1, 0, 0, 0):
#         send_data([nearest_data[0], nearest_data[1], nearest_data[2], nearest_data[3]])
#     else:
#         # Nếu không thấy vật thể, gửi gói dữ liệu trống để robot không bị đợi timeout
#         send_data([255, 0, 0, 0])
#     # Thêm delay nhỏ để giảm tải cho CPU camera khi đứng đợi lệnh


while True:
    fill_light.brightness(100)
    # Kiểm tra xem robot có gửi lệnh yêu cầu lấy dữ liệu không
    if uart.any():
        cmd = uart.read(1) # Đọc 1 byte lệnh từ robot
        if cmd == b'G': # Nếu robot gửi đúng ký tự 'G'
            clock.tick()

            # CHỤP ẢNH NGAY TẠI THỜI ĐIỂM ROBOT YÊU CẦU
            img = sensor.snapshot()

            nearest_data = (-1, 0, 0, 0)
            finding(0)
            finding(1)
            finding(2)
            # finding(3)
            # Gửi dữ liệu mới nhất về ngay lập tức
            if nearest_data != (-1, 0, 0, 0):
                send_data([nearest_data[0], nearest_data[1], nearest_data[2], nearest_data[3]])
            else:
                # Nếu không thấy vật thể, gửi gói dữ liệu trống để robot không bị đợi timeout
                send_data([255, 0, 0, 0])
        if cmd == b'L':
            green_led.on()
