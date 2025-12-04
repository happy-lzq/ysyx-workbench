# Markdown 常用语法速查

Markdown 是一种轻量级标记语言，用于编写易读易写的文档。以下是常用语法：

## 1. 标题
# 一级标题
## 二级标题
### 三级标题

## 2. 文本样式
**粗体文本**
*斜体文本*
~~删除线~~
`行内代码`

## 3. 列表
- 无序列表项 1
- 无序列表项 2

1. 有序列表项 1
2. 有序列表项 2

- [ ] 未完成任务
- [x] 已完成任务

## 4. 代码块
\`\`\`c
// C 语言代码高亮
int main() {
    return 0;
}
\`\`\`

## 5. 链接与图片
[链接文字](URL)
![图片描述](图片URL)

## 6. 引用与分割线
> 这是一个引用块

--- (分割线)

## 7. 表格
| 标题1 | 标题2 |
| --- | --- |
| 内容1 | 内容2 |

---

# GitHub 多账号 SSH 配置指南

在同一台电脑上管理多个 GitHub 账号（例如：个人账号、公司账号、小号）时，经常会遇到 `Permission denied` 错误。这是因为 SSH 默认只使用一个密钥。通过配置 SSH Config，我们可以轻松实现多账号自动切换。

## 1. 生成新的 SSH Key

为你的第二个账号（例如 `happy-lzq`）生成一对新的密钥。不要覆盖默认的 `id_rsa` 或 `id_ed25519`。

```bash
# -C 注释，方便识别
# -f 指定文件名，避免覆盖默认密钥
ssh-keygen -t ed25519 -C "happy-lzq" -f ~/.ssh/id_ed25519_happy
```

执行后，`~/.ssh/` 目录下会多出两个文件：
- `id_ed25519_happy` (私钥，妥善保管)
- `id_ed25519_happy.pub` (公钥，需要上传)

## 2. 配置 SSH Config 文件

编辑或创建 `~/.ssh/config` 文件，告诉 SSH 客户端：**“当连接到某个特定别名时，使用特定的私钥”**。

```bash
# 编辑配置文件
nano ~/.ssh/config
```

添加如下内容：

```ssh
# 账号 1 (默认账号，例如 lzqsyc)
Host github.com
  HostName github.com
  User git
  IdentityFile ~/.ssh/id_ed25519

# 账号 2 (新账号，例如 happy-lzq)
# Host 是一个别名，你可以随便起，比如 github-happy, github-work 等
Host github-happy
  HostName github.com
  User git
  IdentityFile ~/.ssh/id_ed25519_happy
```

**关键点解释：**
- `Host`: 别名，我们在 Git 命令中会用到它。
- `HostName`: 真实的服务器地址，必须是 `github.com`。
- `IdentityFile`: 指定该别名对应的私钥路径。

## 3. 将公钥添加到 GitHub

1.  复制新生成的公钥内容：
    ```bash
    cat ~/.ssh/id_ed25519_happy.pub
    ```
2.  登录 **账号 2 (happy-lzq)** 的 GitHub。
3.  进入 **Settings** -> **SSH and GPG keys** -> **New SSH key**。
4.  粘贴公钥并保存。

## 4. 测试连接

使用 `ssh -T` 命令测试配置是否生效。注意要使用你在 Config 中定义的 **Host 别名**。

```bash
# 测试默认账号
ssh -T git@github.com
# 输出: Hi lzqsyc! ...

# 测试新账号 (使用别名 github-happy)
ssh -T git@github-happy
# 输出: Hi happy-lzq! ...
```

如果看到 `Hi username!`，说明配置成功。

## 5. 在仓库中使用

现在，你需要修改 Git 仓库的远程地址，将域名 `github.com` 替换为你定义的别名 `github-happy`。

**方法 A：克隆新仓库时**
```bash
# 原地址: git@github.com:happy-lzq/repo.git
# 修改为:
git clone git@github-happy:happy-lzq/repo.git
```

**方法 B：修改已有仓库**
```bash
cd my-repo
# 查看当前地址
git remote -v

# 修改 origin 地址
# 格式: git remote set-url origin git@<Host别名>:<用户名>/<仓库名>.git
git remote set-url origin git@github-happy:happy-lzq/ysyx-workbench.git
```

## 总结

配置完成后，Git 的工作流如下：
1. 当你向 `github-happy` 推送时 -> SSH 读取 Config -> 找到 `Host github-happy`。
2. SSH 使用 `~/.ssh/id_ed25519_happy` 私钥进行认证。
3. GitHub 收到请求，识别出这是 `happy-lzq` 的密钥。
4. 鉴权通过，推送成功。

---

# GitHub 多端/多人协同工作流指南

本指南梳理了在多台电脑（如 A 电脑、B 电脑）之间，使用不同 GitHub 账号或同一账号进行协同开发的标准流程。

## 1. 核心概念图解

*   **Remote (远程仓库)**: GitHub 上的仓库，是所有电脑同步的中心。
*   **Local (本地仓库)**: 你电脑上的文件夹。
*   **Push (推送)**: 本地 -> 远程 (上传)。
*   **Pull (拉取)**: 远程 -> 本地 (下载并合并)。

## 2. 场景一：单人多端同步 (A/B 电脑切换)

假设你拥有 `happy-lzq` 账号，白天在 A 电脑工作，晚上回宿舍用 B 电脑继续。

### 流程步骤：

1.  **A 电脑：开始工作**
    *   `git pull` (养成好习惯，开工前先拉取最新代码，防止冲突)
    *   ...写代码，修改文件...
    *   `git add .` (将修改加入暂存区)
    *   `git commit -m "feat: 完成了登录功能"` (保存版本)
    *   `git push` (推送到 GitHub)
    *   *此时 GitHub 上是最新的，A 电脑也是最新的。*

2.  **B 电脑：接力工作**
    *   **关键步骤**: `git pull` (必须先做！把 A 电脑写的代码同步下来)
    *   ...继续写代码...
    *   `git add .`
    *   `git commit -m "fix: 修复了登录 bug"`
    *   `git push`
    *   *此时 GitHub 更新了，B 电脑也是最新的。*

3.  **A 电脑：第二天上班**
    *   **关键步骤**: `git pull` (把昨晚 B 电脑写的代码同步下来)
    *   ...循环往复...

### 常见问题：忘记 Pull 怎么办？
如果你在 B 电脑修改并 Commit 了，准备 Push 时发现报错：
`error: failed to push some refs to ...`
`hint: Updates were rejected because the remote contains work that you do not have locally.`

**原因**: GitHub 上的代码比你本地的新（可能是你在 A 电脑推过的）。
**解决**:
1.  `git pull` (Git 会尝试自动合并 A 和 B 的修改)
2.  如果出现 **Conflict (冲突)**，打开冲突文件，手动保留需要的代码。
3.  解决完冲突后，再次 `git add .` -> `git commit` -> `git push`。

## 3. 场景二：多账号协作 (happy-lzq 与 lzqsyc)

假设仓库属于 `happy-lzq`，但 B 电脑使用的是 `lzqsyc` 账号。

### 权限配置 (前提)
由于仓库是 `happy-lzq` 的，`lzqsyc` 默认没有写入权限。
*   **方法 1 (推荐): 添加协作者 (Collaborator)**
    *   `happy-lzq` 登录 GitHub -> 仓库 Settings -> Collaborators -> Add people -> 邀请 `lzqsyc`。
    *   `lzqsyc` 接受邀请后，就可以直接 `git push` 到这个仓库了。
*   **方法 2: Fork 模式 (开源贡献模式)**
    *   `lzqsyc` 将仓库 Fork 到自己名下。
    *   `lzqsyc` 推送到自己的仓库。
    *   发起 **Pull Request (PR)** 请求合并到 `happy-lzq` 的仓库。

## 4. 进阶：多分支开发工作流

不要一直在 `master` 或 `main` 分支上直接修改，使用分支可以隔离开发任务。

### 推荐流程：

1.  **创建分支 (A 电脑)**
    *   `git checkout -b feature-uart` (创建并切换到 `feature-uart` 分支，开发串口功能)

2.  **开发与推送**
    *   ...写代码...
    *   `git commit -m "wip: 串口初始化"`
    *   `git push -u origin feature-uart` (第一次推送新分支需要 -u)

3.  **切换电脑 (B 电脑)**
    *   `git pull` (拉取远程更新)
    *   `git checkout feature-uart` (切换到这个分支继续开发)
    *   ...写代码...
    *   `git push`

4.  **合并分支 (功能开发完成)**
    *   切换回主分支: `git checkout master`
    *   拉取最新主分支: `git pull`
    *   合并功能分支: `git merge feature-uart`
    *   推送到远程: `git push`
    *   (可选) 删除功能分支: `git branch -d feature-uart`

## 总结图示

```text
[A 电脑] --push--> [GitHub 远                    |
   |                      |                      |
   +-------pull-----------+<-------push----------+
```

**口诀：**
1. **开工先 Pull** (同步别人或自己之前的进度)
2. **收工必 Push** (保存进度到云端)
3. **多用 Branch** (一个功能一个坑，互不影响)
程仓库] --pull--> [B 电脑]
   ^                      |  