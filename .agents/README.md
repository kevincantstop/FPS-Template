# .agents — 项目级 Agent Skills

这个项目（UE 5.8）使用的 Agent Skills 放在这里。DSH 会自动扫描
`<项目根>/.agents/skills/<name>/SKILL.md`（项目级 `project-agents` 根，优先级高于用户级
`~/.agents/skills`），改完不需要重启就会出现在技能目录里。

> 只扫描一层：`skills/<name>/SKILL.md` 或 `skills/<name>.md`。
> 嵌套的子目录（比如 `skills/unreal/unreal-cpp-gameplay`）不会被发现，所以每个技能必须是
> `skills/` 的直接子目录。

## 来源

全部技能来自 [gamedev-skills/awesome-gamedev-agent-skills](https://github.com/gamedev-skills/awesome-gamedev-agent-skills)：

| 项 | 值 |
| --- | --- |
| 克隆位置 | `.agents/sources/awesome-gamedev-agent-skills`（完整 clone，可直接 `git pull`） |
| Commit | `19d93f9`（2026-10-28） |
| 已安装 | `skills/unreal/` 下的 6 个 Unreal Engine 技能，目标版本 **UE 5.8** |

## 已安装的 Unreal 技能

| 技能 | 用途 |
| --- | --- |
| `unreal-cpp-gameplay` | C++ 玩法代码：`UCLASS`/`UPROPERTY`/`UFUNCTION`、Gameplay Framework、`Build.cs` |
| `unreal-blueprints` | 蓝图：Blueprint Class、Event Graph / Construction Script、Cast / Interface / Event Dispatcher |
| `unreal-enhanced-input` | Enhanced Input：Input Action、Mapping Context、Modifier/Trigger、`ETriggerEvent` 绑定 |
| `unreal-behavior-trees` | NPC AI：Behavior Tree + Blackboard、BTTask / BTService、AIController |
| `unreal-niagara` | Niagara VFX：System/Emitter/Module、User 参数、运行时生成 |
| `unreal-packaging` | 打包与 Cook：Development/Shipping、Game Default Map、`RunUAT BuildCookRun` |

技能正文之外还带 `references/` 子文件，`skill` 工具加载主 `SKILL.md` 时会给出资源基路径，
按需读取即可。

## 目录结构

```
.agents/
├── README.md                                  # 本文件
├── skills/                                    # 被 DSH 自动发现的技能根
│   ├── unreal-behavior-trees/
│   │   ├── SKILL.md
│   │   └── references/custom-bttask.md
│   ├── unreal-blueprints/
│   │   ├── SKILL.md
│   │   └── references/communication.md
│   ├── unreal-cpp-gameplay/
│   │   ├── SKILL.md
│   │   └── references/components-and-gc.md
│   ├── unreal-enhanced-input/
│   │   ├── SKILL.md
│   │   └── references/cpp-setup.md
│   ├── unreal-niagara/
│   │   └── SKILL.md
│   └── unreal-packaging/
│       └── SKILL.md
└── sources/
    └── awesome-gamedev-agent-skills/          # 上游仓库 clone（不参与技能发现）
```

## 更新技能

```powershell
cd .agents\sources\awesome-gamedev-agent-skills
git pull
# 回到项目根后重新同步 Unreal 技能（覆盖式拷贝）
Get-ChildItem .agents\sources\awesome-gamedev-agent-skills\skills\unreal -Directory |
  ForEach-Object { Copy-Item $_.FullName -Destination ".agents\skills\$($_.Name)" -Recurse -Force }
```

## 加别的引擎 / 别的技能

上游仓库还有 `disciplines/`、`genres/`、`workflows/`、`godot/`、`unity/`、`web-engines/`、
`other-engines/`、`ebitengine/` 等分组。想用某个技能，把它所在的目录直接拷成
`.agents/skills/<skill-name>` 即可，例如：

```powershell
# 例：加 genres/platformer 和 disciplines/game-feel
Copy-Item .agents\sources\awesome-gamedev-agent-skills\skills\genres\platformer   .agents\skills\ -Recurse
Copy-Item .agents\sources\awesome-gamedev-agent-skills\skills\disciplines\game-feel .agents\skills\ -Recurse
```

也可以直接用上游的 CLI 一次装全量（会自己找 agent 目录）：

```powershell
npx skills add gamedev-skills/awesome-gamedev-agent-skills --list   # 先预览
npx skills add gamedev-skills/awesome-gamedev-agent-skills
```

## 注意

- 这套技能针对 **UE 5.8**，与本工程的 `EngineAssociation` /
  `EngineIncludeOrderVersion.Unreal5_8` 一致；如果以后升/降引擎版本，先看上游
  `docs/VERSION-SUPPORT.md`。
- 技能给的是写法与流程约定（API、类角色、资产命名），不直接改 `.uasset`；蓝图和资产操作
  仍需在编辑器里完成。
