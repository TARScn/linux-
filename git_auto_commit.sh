#!/bin/bash

# git_auto_commit.sh
# 一个自动化执行 git add、git status、git commit 和 git push 的脚本

# 设置默认提交信息
DEFAULT_COMMIT_MSG="update code"

# 获取用户传入的提交信息，如果没有则使用默认值
COMMIT_MSG=${1:-"$DEFAULT_COMMIT_MSG"}

# 显示开始信息
echo "=============================="
echo "开始执行 Git 自动提交脚本"
echo "提交信息: \"$COMMIT_MSG\""
echo "=============================="

# 1. 添加所有更改到暂存区
echo "运行: git add ."
git add .

# 检查 git add 是否成功
if [ $? -ne 0 ]; then
    echo "错误: git add 失败。请检查你的更改并重试。"
    exit 1
fi

# 2. 显示当前 Git 状态
echo "运行: git status"
git status

# 3. 提交更改
echo "运行: git commit -m \"$COMMIT_MSG\""
git commit -m "$COMMIT_MSG"

# 检查 git commit 是否成功
if [ $? -ne 0 ]; then
    echo "错误: git commit 失败。可能没有更改需要提交。"
    exit 1
fi

# 4. 推送更改到远程仓库
echo "运行: git push"
git push

# 检查 git push 是否成功
if [ $? -ne 0 ]; then
    echo "错误: git push 失败。请检查你的网络连接和远程仓库设置。"
    exit 1
fi

# 显示完成信息
echo "=============================="
echo "Git 自动提交并推送完成！"
echo "=============================="

