/*! \file
*   \brief 使用JavaScript创建仿函类型.
*   \author 谭立方
*   \date 2015
*   \version 1.0
*   \copyright 2011-2015PERA Global
*/

log("导入 " + __extension__ + " 插件...");

function registerSimfuns() {
    // the register key is class name.
    if ('Fluent' in rom.library.simfuns)
        throw new Error('Fluent has been in existence.');

    rom.Fluent = function(name, sfParent) {
        var sf = new rom.AutomationScript(name, sfParent);
        sf.derivedName = 'Fluent'; // 指定仿函的派生名称，必须与注册函数名一致
        sf.workDirectory = rom.SimFunction.BaseDirectory;
        var data1 = new rom.FileParameter('AMESim工程文件', rom.Input, sf);
        data1.canDelete = false; // 指定参数不可删除
        data1.fileFilter = "AMESim Project(*.ame)"
        // 提供预置功能
        sf.scriptFunction = function() {

            //log("123");
            //var parentobj = self.parentSimfun;
            //log(parentobj);
            var prjfile = self.parameterByName('AMESim工程文件').fileName;
            if (prjfile == "")
            {
                log("请先创建或导入AMESim工程!");
                return;
            }


            var curfolder = self.workDirectoryByFlag(rom.SimFunction.BaseDirectory);
            var curprjfile = path.join(curfolder,path.filename(prjfile));

            fs.copy(prjfile,curprjfile);

            var appFile = "C:\\AMESim\\v1300\\win32\\AMESim.exe";
            var cp = new ChildProcess();
            cp.spawn(appFile, [curprjfile]);

        };

        return sf;
    };


    // 仿函的构造函数
//    rom.Fluent = function(name, sfParent) {
//        var sf = new rom.AutomationScript(name, sfParent);
//        sf.derivedName = 'Fluent'; // 指定仿函的派生名称，必须与注册函数名一致
//        var data1 = new rom.DoubleParameter('data1', rom.Input, sf);
//        var data2 = new rom.DoubleParameter('data2', rom.Output, sf);
//        data1.canDelete = false; // 指定参数不可删除
//        data2.canDelete = false;
//        // 提供预置功能
//        sf.scriptFunction = function() {
//            log('测试流体计算');
//            self.data2.value = self.data1 + Math.random();
//        };

//        return sf;
//    };

    //仿函的注册
    rom.library.simfuns['Fluent'] = {
        name : '流体计算',
        extension : __extension__, // 模块名称，也即文件夹名称
        composite : false,
        tools : [ // 定制仿函的工具菜单
            {menuItem : '流体计算菜单', icon : 'plotter16.png', actor : function(sf) {
                log('流体计算菜单测试: ', sf.fullName);}},
            {menuItem : 'seperator'},
            {menuItem : '流体计算菜单2', defaultActor : true, actor : function(sf) {
                log('流体计算菜单2测试: ', sf.fullName);}}
        ],
        graphicsItem : 'Item',
        version : '1.0',
        icon : 'fluent.svg', // 设定仿函的图片，image目录下须有16,24结尾的图片文件
        group : 'HySim', // 仿函的分组
        author : '安世亚太科技股份有限公司',
        description : '用于流体计算。',
        os : ['windows', 'linux']
    };

    if ('Dummy' in rom.library.simfuns)
        throw new Error('Dummy has been in existence.');

    // 驱动仿函的构造函数
    rom.Dummy = function(name, sfParent) {
        var assembly = new rom.Assembly(name, sfParent);
        assembly.derivedName = 'Dummy'; // 指定仿函的派生名称，必须与注册函数名一致
        var sf1 = new rom.Fluent('sf1', assembly);

        sf1.scriptFunction = function() {
            self.data2.value = self.data1 + 100 * Math.random();
        };

        var sf2 = new rom.AutomationScript('sf2', assembly);
        var data1 = new rom.DoubleParameter('data1', rom.Input, sf2);
        var data2 = new rom.DoubleParameter('data2', rom.Output, sf2);

        sf2.scriptFunction = function() {
            self.data2.value = self.data1 + 100 * Math.random();
        };

        sf2.createLinkByParameters(data1, sf1.parameters.data2);
        return assembly;
    };

    rom.library.simfuns['Dummy'] = {
        name : 'Dummy驱动仿函',
        extension : __extension__,
        composite : true,
        graphicsItem : 'SequenceItem',
        version : '1.0',
        tools : [
            {menuItem : '驱动仿函菜单', icon : 'plotter16.png', actor : function(sf) {
                log('驱动仿函菜单测试: ', sf.fullName);
            }},
            {menuItem : 'seperator'},
            {menuItem : '驱动仿函菜单2', defaultActor : true, actor : function(sf) {
                log('驱动仿函菜单2测试: ', sf.fullName);}}
        ],
        icon : 'plotter.png',
        group : 'HySim',
        author : '安世亚太科技股份有限公司',
        description : '用于流体计算。',
        os : ['windows', 'linux']
    };
}

__postInit__ = function() {
    log('导入联合仿真中的仿函。');
    log('注册联合仿真中的仿函。');
    registerSimfuns();
};
