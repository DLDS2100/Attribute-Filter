# 洛克王国：世界 — 联防盲点计算器

> 输入队伍精灵属性，快速找出联防覆盖盲区。

## 功能

- 支持 1~6 只精灵配置
- 点击属性按钮弹出 18 属性选择面板
- 自动计算敌方所有属性攻击的受击倍率
- 按 **3× / 2× / 1× / 0.5× / 0.25× / 0×** 分组展示
- 每个属性显示来源精灵，方便调整阵容

## 截图

（待添加）

## 下载

详见 [Releases](../../releases) 页面，下载后解压，双击 `team-defense-calc.exe` 即可运行。

## 编译

### 环境要求

| 组件 | 版本 |
|------|------|
| Qt | 6.11.0（MinGW 64-bit） |
| MinGW | 13.1.0（配套 Qt 6.11.0） |
| 编译器 | g++ 支持 C++17 |

### 步骤

```bat
git clone https://github.com/<你的用户名>/team-defense-calc.git
cd team-defense-calc
build.bat
```

编译成功后 exe 位于 `build\team-defense-calc.exe`，双击 `build\run.bat` 可带控制台运行（方便调试）。

### 打包发布

```bat
REM 在 build\ 目录下运行 windeployqt 收集依赖
F:\Qt\6.11.0\mingw_64\bin\windeployqt.exe build\team-defense-calc.exe
REM 然后把整个 build\ 目录压缩即可分发
```

## 数据来源

属性克制表来自 [洛克王国世界 BWIKI](https://wiki.biligame.com/rocom)，以 `TypeChart.json` 格式内置在程序中。

## 许可证

MIT
