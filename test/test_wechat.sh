echo "=== 1. 系统信息 ==="
sw_vers
uname -m

echo -e "\n=== 2. 当前用户和会话 ==="
who
stat -f "%Su" /dev/console

echo -e "\n=== 3. 微信进程 ==="
pgrep -lx WeChat || echo "微信未运行"

echo -e "\n=== 4. 屏幕状态 ==="
python3 -c "import Quartz; d=Quartz.CGSessionCopyCurrentDictionary(); print(d)" 2>/dev/null || echo "无法获取会话信息"

echo -e "\n=== 5. 辅助功能权限（你的程序） ==="
# 替换成你的二进制路径
tccutil 2>&1 | head -5

# 测试 2：激活微信（用 System Events）
echo "=== 测试 2 ==="
osascript -e 'tell application "System Events" to set frontmost of process "WeChat" to true' 2>&1
echo "Exit: $?"

# 测试 3：激活微信（用 WeChat 自己的 activate）
echo -e "\n=== 测试 3 ==="
osascript -e 'tell application "WeChat" to activate' 2>&1
echo "Exit: $?"

# 测试 4：查看微信窗口
echo -e "\n=== 测试 4 ==="
osascript -e 'tell application "System Events" to get name of windows of process "WeChat"' 2>&1

# 测试 5：查看微信窗口的更多属性
echo -e "\n=== 测试 5 ==="
osascript -e 'tell application "System Events" to tell process "WeChat" to get {name, visible, frontmost} of every window' 2>&1
