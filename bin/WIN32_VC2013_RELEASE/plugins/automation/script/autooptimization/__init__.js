/*! \file
*   \brief 支持优化算法仿函注册与创建代码.
*   \author 谭立方
*   \date 2015
*   \version 1.0
*   \copyright 2011-2015 PERA Global
*/

log(qsTranslate("AutoScript", "Import ") + __extension__ + qsTranslate("AutoScript", " plugin..."));
__setupPackage__("rom.autooptimization");

function setOptimizationProperties(sf, props) {
    rom.$.setSimfunProperties(sf, props);
    sf.maxIteration = props.maxIteration;
    sf.tolerance = props.tolerance;
    sf.designVariables = props.designVariables;
    sf.constraints = props.constraints;
    sf.objectives = props.objectives;
    sf.dissatisfiedSet = props.dissatisfiedSet;
    sf.optimumSet = props.optimumSet;
}

function createPSO(model, parentSf) {
    var sf = new rom.PSO(model.properties.id, parentSf);
    var props = model.properties;
    setOptimizationProperties(sf, props);
    sf.w = props.w;
    sf.c1 = props.c1;
    sf.c2 = props.c2;
    sf.iterationCount = props.iterationCount;
    sf.particlesCount = props.particlesCount;
    return sf;
}

function registerSimfuns() {
    if ('PSO' in rom.library.simfuns)
        throw new Error(qsTranslate('AutoScript', 'PSO has been in existence.'));

    rom.library.simfuns['PSO'] = {
        name : qsTranslate('AutoScript', 'PSO'),
        extension : __extension__,
        factory : createPSO,
        composite : true,
        graphicsItem : 'LoopItem',
        plugin : 'AutoOptimizationGUI',
        tools : [ /*{ menuItem : qsTranslate('AutoScript', 'Loop Editor...'), actor : 'LoopEditor', defaultActor : true, //modedialog默认为true
                    statusTip : qsTranslate('AutoScript', "Open a dialog to edit SimFunction's properties") },*/
//                 { menuItem : qsTranslate('AutoScript', 'Diagram...'), actor : 'LoopPlotters', modedialog : false,
//                    icon : 'plotter.png', statusTip : qsTranslate('AutoScript', "Open a dialog to edit diagram's properties") },
//                 { menuItem : 'seperator' },
            { menuItem : qsTranslate('AutoScript', 'History Data Browsing'), icon : 'history.png', actor : 'IterDataBrowsing', constraint : 1},
            { menuItem : qsTranslate('AutoScript', 'Realtime Data Browsing'), icon : 'realtime.png', actor : 'RealtimeDataBrowsing', constraint : 2}
//                 { menuItem : qsTranslate('AutoScript', 'Numerical Parameter Data Extraction...'), actor : 'NumberExporter', modedialog : true }],
        ],
        icon : 'pso.svg',
        group : qsTranslate('AutoScript', 'Base Driver SimFunction'),
//        group : qsTranslate('AutoScript', 'Optimization Algorithm'),
        author : qsTranslate('AutoScript', 'peraglobal'),
        description : qsTranslate('AutoScript', 'The PSO SimFunction is used to optimize a model.'),
        os : ['windows', 'linux']
    };

}

__postInit__ = function() {
    log(qsTranslate('AutoScript', 'AutoOptimization is Imported'));
    log(qsTranslate('AutoScript', 'Register AutoOptimization SimFunction'));
    registerSimfuns();
}
