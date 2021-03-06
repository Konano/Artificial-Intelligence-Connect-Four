# 四子棋作业实验报告

## 概况

- 本文是一篇实验报告，主要介绍四子棋 AI 算法方面的思考和设计。

- 算法主体为蒙特卡洛树搜索，并在其中加入了 UCT 置信公式辅助选择子结点。

- 目前算法在 1 秒时限内与测试集的 50 个 dll 随机对战时胜率高达 95.6%，与 92,94,96,98,100 编号的 dll 对战时胜率高达 90%


## 算法设计

### UCT 改良

算法为最为基础的蒙特卡洛树搜索，其中选择策略中运用了 UCT。这部分直接按照维基百科上所描述的实现即可。然而在实际运用中 UCT 也存在其问题，那就是在开头尝试次数过于少时容易产生的「偏见」。

UCT 每次选取的都是具有表达式最大值的子结点，但最后结果的选定还是会看各个子结点的胜率。所以会出现这样一种情况：某次优子结点在一开始被多次选中，其次数和胜率都比最优子结点高；到了中后期，最优子结点在 UCT 的补助下仍然会被选中几次，但其胜率增加量要小于次数增加所导致的补助量的下降，这就导致最优子结点无法被多次选中，其胜率无法通过多次尝试而纠正，从而使得算法最后选择了次优子结点。

对此我采取的解决方案是：假如有子结点未扩展过，则采取随机选择子结点扩展；当全部子结点都扩展了至少一遍后，使用 UCT 选择子结点扩展。实践发现这样能较大程度避免上文提到的「偏见」，但还是无法避免。

### 制胜位

对于游戏本身，我引入了一个概念：「制胜位」。一个格子是某玩家的制胜位，当且仅当该玩家把棋子放在这个格上时能形成四子连棋。

对于一个局面我们可以显然得到以下结论：

- 假如我有制胜位，那么我直接把棋子下到制胜位即可取得胜利
- 假如我没有制胜位，且对方有两个制胜位，则直接告负，不继续扩展
- 假如我没有制胜位，且对方只有一个制胜位，那我必将把棋放置在对方的制胜位上

这样便能去除大量无用的扩展节点，使得搜索范围更加广泛，搜索深度更加深。

## 实验结果

我用改良后的算法随机选择测试库中的某个 dll 进行对战，共进行 250 轮。

其在对战不大于 30 的 dll 中做到了全胜，在 30-90 的区间的 dll 则偶尔会丢掉一局，在 90-100 则有 90% 的胜率。

|      | (0,10] | (10,20] | (20,30] | (30,40] | (40,50] | (50,60] | (60,70] | (70,80] | (80,90] | (90,100] |
| ---- | ------ | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | -------- |
| 胜率 | 100%   | 100%    | 100%    | 98%     | 98%     | 90%     | 98%     | 88%     | 96%     | 90%      |

由于对战局数少，此处概率并不能准确表现出算法的真实水平。

## 总结

算法在未经过常数优化，甚至是在使用一半不到的时限内，就已经做到如此高的水平，这大大出乎我的预料。

对于这类小型游戏求解，蒙特卡洛树搜索跑 0.5s 和 3s 出来的结果其实优劣相差甚少，所以我预感常数优化应该不会给算法带来巨大的飞跃。

反而重要的是算法本身。对选择策略进行了改良后，被忽视的最优子结点的数量大大减小；加入了「制胜位」之后，搜索质量更是直接上了一个台阶。这两个改进直接使得胜率从 40% 上升到 95%。

代码：https://github.com/Konano/homework-connect-four