/*
 *动态创建模型
 *初始模型只是一个根模型, 当模型运行时通过根节点的前处理器将创建新的模型元素: 一个AutomationScript和若干个参数.
 *同时要注意, 在Assembly前处理器中创建了AutomationScript和两个双精度参数, 在AutomationScript的执行过程中另外创建了一个
 *组参数和一个双精度参数.
 */

rom.model = new rom.Assembly('model', null);

rom.model.preProcessingFunction = function() {
    if (!self.as) { // 避免每次运行都创建, self 代表 rom.model
        self.dynamicModifyModel(function() { // 模型元素在模型执行环境中的创建需使用dynamicModifyModel方法 
            var as = new rom.AutomationScript('as', self);
            var data1 = new rom.DoubleParameter('data1', rom.Input, as);
            var data2 = new rom.DoubleParameter('data2', rom.Output, as);

            as.scriptFunction = function() {//此函数中的self代表 rom.model.as
                var abc = 10; 
                self.data2.value = abc + self.data1.value + 10*Math.random();
                var group = self.group;
                if (!group) {
                    group = self.dynamicModifyModel("new rom.GroupParameter('group', self)");
                    data3 = self.dynamicModifyModel(function() {
                        group.description = '组参数的描述';
                        new rom.DoubleParameter('data3', rom.Input, group);
                    });
                    data3.description = '双精度浮点数参数';
                }
                data3.value = Math.random()*10;
            };
       });
    }
};
