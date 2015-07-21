/*
 *仿函的取消
 *测试模型是一个脚本仿函，只有一个输出双精度浮点数参数，通过随机函数产生一个0-100的值。
 *脚本仿函有一个后处理器函数，判断脚本执行后的输出是否小于60，如果是则取消当前仿函的执行，
 *此时计算结果不会存入数据库中。
 *同时在执行后动态创建一个整型参数作为执行次数的计数器，当结果不小于60时，每执行一次计数器加1
 */

rom.model = new rom.Assembly('model', null);
var sf = new rom.AutomationScript('取消', rom.model);
var data = new rom.DoubleParameter('data', rom.Output, sf);
sf.scriptFunction = function() {
    self.data.value = Math.random() * 100;
    log(self.data);
}

sf.postProcessingFunction = function() {
    var counter = self.counter;
    if (!counter) {
        counter = self.dynamicModifyModel("new rom.IntegerParameter('counter', rom.Output, self)");
        counter.value = 0;
        counter.description = '仿函执行次数';
    }

    if (self.data.value < 60) {
        self.cancel = true;
    } else {
        counter.value += 1;
    }
}