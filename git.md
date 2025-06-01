# 这是简单学习的视频网址
// [简单使用学习链接](https://www.bilibili.com/video/BV1e541137Tc/?spm_id_from=333.1387.homepage.video_card.click&vd_source=76bb23ac20e722febedec8063c0922e0)

# 这是入门学习的视频网址
// [入门使用学习链接](https://www.bilibili.com/video/BV1db4y1d79C/?spm_id_from=333.788.comment.all.click&vd_source=76bb23ac20e722febedec8063c0922e0)

# 这是深入学习的图文网址
// [深入使用学习链接](https://blog.csdn.net/qq_16027093/article/details/130503317)

# 参考学习图文网址
// [参考学习](https://www.runoob.com/git/git-tutorial.html)

# 配置
git config --global user.name "your name"
git config --global user.email your_email@xxx
ssh-keygen -t rsa -C "your_email@xxx"
// 写入路径
// 写入密码

# 克隆
git clone object_url					// 拉取至当前目录
git clone object_url directory_name		// 拉取至指定目录

git fetch --all					// 拉取所有分支
git fetch origin FZ_name		// 拉去特定分支

# 查看
git log --stat					// 查看更改记录

# 更新
git add -A 						// 更新保存
git commit -m "提交备注"			// 保存进仓库
git push -u origin FZ_name		// 更新当前分支保存远程仓库
git push origin FZ_name			// 更新当前分支并进行合并
git push -f origin FZ_name		// 强制推送
git push --delete origin FZ_name// 删除远程分支

git push			// 更新至远端
git pull			// 更新至本地

# 维护
git checkout filename			// 撤回工作区文件更改
git reset HEAD^					// 撤回以提交文件更改

# 分支
git checkout -b FZ_name			// 在当前分支创建新分支
git branch FZ_name				// 同上

git branch						// 列举所有分支
git checkout FZ_name			// 切换至另一分支
git branch -D FZ_name			// 删除分支
git merge FZ_name				// 合并目标分支和当前分支