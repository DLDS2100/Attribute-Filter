"""
生成属性盲点计算器图标 (app_icon.png)
设计：蓝色盾牌 + 白色闪电/属性符号
尺寸：256×256 (同时生成 32×32 供窗口图标使用)
"""

from PIL import Image, ImageDraw, ImageFont
import math
import os

def draw_shield_icon(size=256):
    img = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)

    s = size
    pad = s * 0.08  # 边距

    # ── 1. 盾牌路径 ──
    # 盾牌：顶部为弧形矩形，底部收拢成尖
    pts = []
    # 顶部左圆角
    r = s * 0.12
    top    = pad
    bottom = s * 0.94
    left   = pad
    right  = s - pad
    cx     = s / 2

    # 顶部弧（从左上圆角开始，顺时针）
    def arc_pts(cx, cy, r, start_deg, end_deg, steps=16):
        out = []
        for i in range(steps + 1):
            t = math.radians(start_deg + (end_deg - start_deg) * i / steps)
            out.append((cx + r * math.cos(t), cy + r * math.sin(t)))
        return out

    # 左上圆角
    pts += arc_pts(left + r, top + r, r, 180, 270)
    # 顶边 → 右上圆角
    pts += arc_pts(right - r, top + r, r, 270, 360)
    # 右边，沿矩形右侧下来，然后向内收拢成尖
    mid_y = s * 0.62       # 开始收拢的 Y
    pts.append((right, mid_y))
    # 收拢到底部尖
    pts.append((cx, bottom))
    # 左边上来
    pts.append((left, mid_y))
    # 回到左上圆角起点
    pts.append((left, top + r))

    # ── 2. 绘制盾牌阴影 ──
    shadow_pts = [(x + s*0.02, y + s*0.025) for x, y in pts]
    draw.polygon(shadow_pts, fill=(30, 80, 160, 80))

    # ── 3. 绘制盾牌渐变（用多层填充模拟）──
    # 从亮蓝 #4A90D9 到深蓝 #1A4A8A
    layers = 40
    for i in range(layers):
        t = i / (layers - 1)
        r_c = int(74  + (26  - 74)  * t)
        g_c = int(144 + (74  - 144) * t)
        b_c = int(217 + (138 - 217) * t)
        # 每层稍微缩进
        shrink = i * 0.3
        layer_pts = []
        for px, py in pts:
            dx = px - cx
            dy = py - (s * 0.5)
            dist = math.hypot(dx, dy)
            if dist > 0:
                nx = px - shrink * dx / max(dist, 1)
                ny = py - shrink * dy / max(dist, 1)
            else:
                nx, ny = px, py
            layer_pts.append((nx, ny))
        draw.polygon(layer_pts, fill=(r_c, g_c, b_c, 255))

    # ── 4. 盾牌高光（顶部左侧亮光）──
    hl_pts = [
        (left + r*0.5,       top + r*1.2),
        (cx * 0.72,          top + r*0.8),
        (cx * 0.75,          s * 0.38),
        (left + r*0.8,       s * 0.40),
    ]
    draw.polygon(hl_pts, fill=(255, 255, 255, 45))

    # ── 5. 盾牌边框 ──
    draw.line(pts + [pts[0]], fill=(255, 255, 255, 120), width=max(2, s//80))

    # ── 6. 闪电符号（中间白色）──
    # 闪电：上半部分向右，下半部分向左，经典 Z 形
    lx = cx
    ly_top    = s * 0.20
    ly_bottom = s * 0.78
    ly_mid    = s * 0.50
    hw = s * 0.13   # 横向宽度

    bolt = [
        (lx + hw * 0.6,  ly_top),           # 右上
        (lx - hw * 0.2,  ly_mid - s*0.02),  # 左中偏上
        (lx + hw * 0.35, ly_mid - s*0.02),  # 中间右
        (lx - hw * 0.6,  ly_bottom),        # 左下
        (lx + hw * 0.2,  ly_mid + s*0.02),  # 右中偏下
        (lx - hw * 0.35, ly_mid + s*0.02),  # 中间左
    ]

    # 闪电阴影
    bshadow = [(x + s*0.012, y + s*0.012) for x, y in bolt]
    draw.polygon(bshadow, fill=(0, 40, 120, 100))
    # 闪电本体
    draw.polygon(bolt, fill=(255, 255, 255, 240))
    # 闪电高光
    draw.polygon(bolt, outline=(255, 255, 200, 180), width=max(1, s//128))

    # ── 7. 星形光晕（闪电尖端）──
    star_cx, star_cy = lx + hw * 0.6, ly_top
    star_r_out = s * 0.035
    star_r_in  = star_r_out * 0.45
    star_pts = []
    for i in range(8):
        angle = math.radians(-90 + i * 45)
        r_use = star_r_out if i % 2 == 0 else star_r_in
        star_pts.append((star_cx + r_use * math.cos(angle),
                         star_cy + r_use * math.sin(angle)))
    draw.polygon(star_pts, fill=(255, 240, 120, 200))

    return img


def main():
    out_dir = os.path.join(os.path.dirname(__file__), "resources")
    os.makedirs(out_dir, exist_ok=True)

    # 256×256
    img256 = draw_shield_icon(256)
    path256 = os.path.join(out_dir, "app_icon.png")
    img256.save(path256, "PNG")
    print(f"Generated: {path256}")

    # 32×32（窗口图标）
    img32 = img256.resize((32, 32), Image.LANCZOS)
    path32 = os.path.join(out_dir, "app_icon_32.png")
    img32.save(path32, "PNG")
    print(f"Generated: {path32}")

    # 生成 ICO（含多种尺寸，用于 Windows 窗口）
    ico_path = os.path.join(out_dir, "app_icon.ico")
    img_ico = img256.copy()
    img_ico.save(ico_path, format="ICO",
                 sizes=[(256,256),(128,128),(64,64),(48,48),(32,32),(16,16)])
    print(f"Generated: {ico_path}")


if __name__ == "__main__":
    main()
