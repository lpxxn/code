/*
 *仿函的重做
 *测试模型是一个脚本仿函，只有一个输出双精度浮点数参数，通过随机函数产生一个0-100的值。
 *脚本仿函有一个后处理器函数，判断脚本执行后的输出是否小于95，如果是则重新执行当前的脚本仿函，
 *同时在第一次执行后动态创建一个整型参数作为执行次数的计数器，每执行一次计数器加1，
 *这个过程将持续到输出值不小于95.
 */

rom.model = new rom.Assembly('model', null);
var sf = new rom.AutomationScript('重做', rom.model);
var data = new rom.DoubleParameter('data', rom.Output, sf);
sf.scriptFunction = function() {
    self.data.value = Math.random() * 100;
}

sf.postProcessingFunction = function() {
    var counter = self.counter;
    if (!counter) {
        counter = self.dynamicModifyModel("new rom.IntegerParameter('counter', rom.Output, self)");
        counter.value = 0;
        counter.description = '仿函执行次数';
    }
    counter.value += 1;

    if (self.data.value < 95) {
        self.redo = true;
    }
}
