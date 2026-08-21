on run argv
    try
        set chatTool to item 1 of argv
        set contactName to item 2 of argv
        set messageText to item 3 of argv

        -- 检查是否在运行
        tell application "System Events"
            set chatToolRunning to (exists (processes whose name is chatTool))
        end tell
        if not chatToolRunning then
            error "通讯软件未运行，请先启动" number 1001
        end if

        tell application chatTool to activate
        delay 0.8

        -- 进入前台
        tell application "System Events"
            tell process chatTool
                set frontmost to true
            end tell
        end tell
        delay 0.8

        tell application "System Events"
            -- 搜索联系人
            keystroke "f" using {command down}
            delay 0.3
            set the clipboard to contactName
            keystroke "v" using {command down}
            delay 0.8
            key code 36
            delay 0.8

            -- 聚焦输入框(输入down，再输入up)
            key code 125
            delay 0.3
            key code 126
            delay 0.3

            -- 发送消息
            set the clipboard to messageText
            keystroke "v" using {command down}
            delay 0.2
            key code 36
        end tell

        return "OK"

    on error errMsg number errNum
        -- 统一错误输出格式，方便调用方解析
        return "ERROR:" & errNum & ":" & errMsg
    end try
end run
