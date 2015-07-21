/*
 *动态创建模型
 *初始模型是一个脚本仿函，只有一个输出双精度浮点数参数，通过随机函数产生一个0-100的值。
 *脚本仿函有一个后处理器函数，判断脚本执行后的输出是否小于95，如果是则动态创建当前的脚本仿函，
 *并把它插入模型的后面，模型执行引擎将接着执行动态创建的脚本仿函，如果输出值还小于95，那么还将创建新的脚本仿函，
 *这个过程将持续到输出值不小于95.
 */

rom.model = new rom.Assembly('model', null);
var sf = new rom.AutomationScript('考试', rom.model);
var data = new rom.DoubleParameter('data', rom.Output, sf);
sf.scriptFunction = function() {
    self.data.value = Math.random() * 100;
}

sf.postProcessingFunction = function() {
    if (self.data.value < 95) {
        self.dynamicModifyModel(function() {
            var clonedSf = rom.$.cloneSimfunByJSON('{' + self.toJSON() + '}');
            clonedSf.name = '考试' + rom.model.simfunCount();
            rom.model.insert(rom.model.simfunCount() - 1, clonedSf);
        });
    }
}				
