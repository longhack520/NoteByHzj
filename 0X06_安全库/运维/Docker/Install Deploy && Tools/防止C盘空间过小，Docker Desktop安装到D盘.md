Docker经常把C盘空间占满，于是想办法将docker整体迁移到d盘
<a name="k9dWn"></a>
## 安装

- Docker官网下载docker desktop installer.exe安装程序。
- 进入下载目录，使用cmd直接执行以下命令(**PowerShell官方给出的执行会报错**)。
```powershell
start /w "" "Docker Desktop Installer.exe" install --installation-dir=D:\Docker
```
```powershell
C:\Users\Dell\Downloads>start /w "" "Docker Desktop Installer.exe" install --installation-dir=D:\Docker
-------------------------------------------------------------------------------->8
Version: 4.23.0 (120376)
Sha1:
Started on: 2023/09/22 08:32:53.946
Resources: C:\Users\Dell\Downloads\resources
OS: Windows 10 Pro
Edition: Professional
Id: 2009
Build: 19045
BuildLabName: 19041.1.amd64fre.vb_release.191206-1406
File: C:\ProgramData\DockerDesktop\install-log-admin.txt
CommandLine: "Docker Desktop Installer.exe"  install --installation-dir=D:\Docker
You can send feedback, including this log file, at https://github.com/docker/for-win/issues
[2023-09-22T08:32:55.060397300Z][ManifestAndExistingInstallationLoader][I] Install path is D:\Docker. Loading manifest first
[2023-09-22T08:32:55.070975900Z][ManifestAndExistingInstallationLoader][I] No manifest found, returning no existing install
[2023-09-22T08:32:55.070975900Z][Installer][I] No installation found
[2023-09-22T08:32:55.298155600Z][InstallWorkflow][I] Cancel pending background download
[2023-09-22T08:32:55.300158200Z][BackgroundTransfer][I] Cancel current background transfer job
[2023-09-22T08:32:55.311024400Z][InstallWorkflow][I] Using package: res:DockerDesktop
[2023-09-22T08:32:55.316023700Z][InstallWorkflow][I] Downloading
[2023-09-22T08:32:56.910007700Z][InstallWorkflow][I] Extracting manifest
[2023-09-22T08:32:57.709791600Z][InstallWorkflow][I] Manifest found: version=120376, displayVersion=4.23.0, channelUrl=https://desktop-stage.docker.com/win/main/amd64/appcast.xml
[2023-09-22T08:32:57.709791600Z][InstallWorkflow][I] Checking prerequisites
[2023-09-22T08:32:57.997653000Z][InstallWorkflow][I] Prompting for optional features
[2023-09-22T08:33:05.435792600Z][InstallWorkflow][I] Selected backend mode: wsl-2
[2023-09-22T08:33:05.436792300Z][InstallWorkflow][I] Unpacking artifacts
[2023-09-22T08:34:36.935634700Z][InstallWorkflow][I] Deploying component CommunityInstaller.CreateGroupAction
[2023-09-22T08:34:43.917420500Z][InstallWorkflow][I] Deploying component CommunityInstaller.AddToGroupAction
[2023-09-22T08:34:43.937424700Z][InstallWorkflow][I] Deploying component CommunityInstaller.EnableFeaturesAction
[2023-09-22T08:34:43.959017600Z][InstallWorkflow-EnableFeaturesAction][I] Required features: VirtualMachinePlatform, Microsoft-Windows-Subsystem-Linux
[2023-09-22T08:34:44.335984300Z][InstallWorkflow][I] Deploying component CommunityInstaller.ServiceAction
[2023-09-22T08:34:44.338985100Z][InstallWorkflow-ServiceAction][I] Removing service
[2023-09-22T08:34:44.339985900Z][InstallWorkflow-ServiceAction][I] Creating service
[2023-09-22T08:34:44.455986300Z][InstallWorkflow][I] Deploying component CommunityInstaller.ShortcutAction
[2023-09-22T08:34:44.481022400Z][InstallWorkflow-ShortcutAction][I] Creating shortcut: C:\ProgramData\Microsoft\Windows\Start Menu\Docker Desktop.lnk/Docker Desktop
[2023-09-22T08:34:44.500015400Z][InstallWorkflow][I] Deploying component CommunityInstaller.ShortcutAction
[2023-09-22T08:34:44.502020100Z][InstallWorkflow-ShortcutAction][I] Creating shortcut: C:\Users\Dell\Desktop\Docker Desktop.lnk/Docker Desktop
[2023-09-22T08:34:44.507985600Z][InstallWorkflow][I] Deploying component CommunityInstaller.AutoStartAction
[2023-09-22T08:34:44.681985500Z][InstallWorkflow][I] Deploying component CommunityInstaller.PathAction
[2023-09-22T08:34:44.835984600Z][InstallWorkflow][I] Deploying component CommunityInstaller.ExecAction
[2023-09-22T08:34:44.838984400Z][InstallWorkflow-ExecAction][I] Running: D:\Docker\InstallerCli.exe -i with timeout=-1
[2023-09-22T08:34:46.135828700Z][InstallWorkflow][I] Registering product
[2023-09-22T08:34:46.184831600Z][InstallWorkflow][I] Deleting C:\ProgramData\DockerDesktop\install-settings.json
[2023-09-22T08:34:46.186837900Z][InstallWorkflow][I] Installation succeeded
```
<a name="V6BLy"></a>
## 设置镜像地址到D盘

- 如果想要将 Docker Desktop 的默认虚拟磁盘镜像(Disk image)位置更改至 D 盘，可以按照以下步骤进行操作：
- 在系统托盘中找到 Docker Desktop 图标，右键单击并选择“Settings”。
- 在左侧导航栏中选择“Resources”选项卡。
- 找到“Disk image location”选项，并点击它右侧的“Edit”按钮。
- 在打开的对话框中输入 D 盘的路径，例如D:\Docker\disk。
- 点击 “Apply & Restart” 按钮保存更改并重启 Docker Desktop。
- 现在，Docker Desktop 的默认虚拟磁盘镜像位置就被更改为了 D 盘，你可以在此路径下查看和管理 Docker Desktop 的虚拟磁盘镜像了。

请注意，更改虚拟磁盘镜像位置可能会对已有的容器和镜像造成影响，请在操作前备份好数据，以免造成数据丢失。
