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

# Tắt tự động cân bằng trắng/sáng để màu sắc không bị nhảy thất thường khi robot di chuyển
# sensor.set_auto_gain(False)
# sensor.set_auto_whitebal(False)

fill_light.on()

threshold_obj = [
    (26, 18, -128, -15, 127, -128),
    (26, 0, 39, 127, -128, 38),
    (33, 0, 33, 127, -128, 127),
    (30, 8, -128, -14, 127, -128),
    (22, 0, 16, 127, -128, 127)
]

clock = time.clock()

nearest_data = (-1, 0, 0, 0)

# 1. Hạ ngưỡng độ phủ xuống 50% (0.5) để không bị mất dấu khi robot lắc/di chuyển
MIN_DENSITY_THRESHOLD = 0.5

# 2. Cơ chế nhớ vị trí (Chống mất dấu đột ngột)
last_valid_data = (-1, 0, 0, 0)
lost_frames_count = 0
MAX_LOST_FRAMES = 4  # Cho phép mất dấu tối đa 3 frame liên tiếp trước khi dừng robot

def finding(color_id, img):
    global nearest_data

    # Ép tham số lọc nhiễu cứng cáp ngay từ đầu
    blobs = img.find_blobs(
        [threshold_obj[color_id]],
        pixels_threshold=300,   # Hạ bớt để nhận diện được khi vật thể ở xa
        area_threshold=200,
        merge=False,            # KHÔNG gộp blob để tránh phình to
        x_stride=2,
        y_stride=2
    )

    if blobs:
        mapped_color_id = color_id
        if color_id in (2, 4):
            mapped_color_id = 1
        elif color_id == 3:
            mapped_color_id = 0

        for blob in blobs:
            # Tính độ phủ màu
            bbox_area = blob.w() * blob.h()
            density = blob.pixels() / bbox_area if bbox_area > 0 else 0

            # Lọc bỏ nhiễu quá loãng (< 50%)
            if density < MIN_DENSITY_THRESHOLD:
                continue

            x_center = blob.cx()
            y_center = blob.cy()
            blob_area = round(blob.area() / 2)

            # Draw visual
            img.draw_rectangle(blob.rect())
            img.draw_cross(x_center, y_center)

            # Lựa chọn vật thể gần robot nhất (Y lớn nhất)
            if y_center > nearest_data[2]:
                nearest_data = (mapped_color_id, x_center, y_center, blob_area)

while True:
    fill_light.brightness(100)

    if uart.any():
        cmd = uart.read(1)
        if cmd == b'G':
            clock.tick()

            img = sensor.snapshot()

            # Khử nhiễu đốm nhỏ bằng thuật toán Morphological Op (Open)
            # img.open(1)

            nearest_data = (-1, 0, 0, 0)

            # Tìm kiếm vật thể
            finding(0, img)
            finding(1, img)
            finding(2, img)
            finding(3, img)
            finding(4, img)
            # --- XỬ LÝ DỮ LIỆU LIÊN TỤC (TRACKING LOGIC) ---
            if nearest_data[0] != -1:
                # Nếu tìm thấy: Cập nhật dữ liệu mới & reset bộ đếm mất dấu
                last_valid_data = nearest_data
                lost_frames_count = 0
                send_data([nearest_data[0], nearest_data[1], nearest_data[2], nearest_data[3]])
            else:
                # Nếu tạm thời mất dấu ở frame này:
                if lost_frames_count < MAX_LOST_FRAMES and last_valid_data[0] != -1:
                    # Tận dụng vị trí cũ để robot tiếp tục lao tới chứ không dừng khựng
                    lost_frames_count += 1
                    send_data([last_valid_data[0], last_valid_data[1], last_valid_data[2], last_valid_data[3]])
                else:
                    # Mất dấu thật sự (> 3 frame): Mới báo trống về cho robot
                    send_data([255, 0, 0, 0])

        elif cmd == b'L':
            green_led.on()
