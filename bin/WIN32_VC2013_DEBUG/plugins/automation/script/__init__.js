/*! \file
*   \brief 自动化脚本初始化代码.
*   \author 谭立方
*   \date 2011-2014
*   \version 1.0
*   \copyright 2011-2014PERA Global
*/

Array.prototype.in_array = function(elem) {
    for(var i = 0; i < this.length; i++) {
        if(this[i] === elem)
            return true;
    }
    return false;
}

function clone(object) {
    function F() {}
    F.prototype = object;
    return new F;
}

// parameter mode
rom.Input = rom.IntegerParameter.Input;
rom.Output = rom.IntegerParameter.Output;
rom.Linked = rom.IntegerParameter.Linked;
rom.Group = rom.IntegerParameter.Group;
rom.InputReference = rom.IntegerParameter.InputReference;
rom.OutputReference = rom.IntegerParameter.OutputReference;
rom.InputComposite = rom.IntegerParameter.ComInput;
rom.OutputComposite = rom.IntegerParameter.ComOutput;
rom.InputFile = rom.IntegerParameter.InputFile;
rom.OutputFile = rom.IntegerParameter.OutputFile;

// WorkDirectory
rom.BaseDirectory = rom.SimFunction.BaseDirectory;
rom.InputDirectory = rom.SimFunction.InputDirectory;
rom.OutputDirectory = rom.SimFunction.OutputDirectory;

// load all available extensions
var libs = process.availableExtensions;
for (var i = 0; i < libs.length; ++i) {
    try {
        process.importExtension(libs[i]);
    } catch (e) {
        log(e);
    }
}

rom.$ = {
    /*
     * 设置模型元素的基础属性.
     */
    setModelElementProperties : function(me, properties) {
        if ('valid' in properties)
            me.valid = properties.valid;
        if ('save' in properties)
            me.saveToDataBase = properties.save;
        if ('canDelete' in properties)
            me.canDelete = properties.canDelete;
        if ('description' in properties)
            me.description = properties.description;
        if ('respondChangedCode' in properties)
            me.respondChangedCode = properties.respondChangedCode;
        if ('actionList' in properties)
            me.actionListCode = properties.actionList;
    },

    /*
      * 设置仿函的基础属性.
      */
    setSimfunProperties : function(sf, properties) {
        rom.$.setModelElementProperties(sf, properties);
        sf.preProcessingCode = properties.preProcessingCode;
        sf.postProcessingCode = properties.postProcessingCode;
        sf.workDirectory = properties.workDirectory;
        sf.additionalIconFilePath = properties.additionalIconFilePath;
        sf.derivedName = properties.derivedName;
    },

    parseParameter : function(name, parameter, sf, parObj) {
        var parameterClass = parameter.parameterClass;
        var factory = rom.library.parameters[parameterClass].factory;
        return factory(name, parameter, sf, parObj);
    },

    parseLink : function(name, link, sf) {
        sf.createLinkByNames(name, link);
    },

    parseModel : function(model, parentSf) {
        var simfunClass = model.simfunClass;
        var Factory = rom.library.simfuns[simfunClass].factory;
        var sf = new Factory(model, parentSf);

        if ('parameters' in model) {
            var parameters = model.parameters;
            for (var name in parameters) {
                var parObj = null;
                if (sf.containParameter(name)) {
                    parObj = sf.parameterByName(name);
                }
                rom.$.parseParameter(name, parameters[name], sf, parObj);
            }
        }
        // must be created before links, since some parameters be created auto, eg. Switch.
        if ('children' in model) {
            var children = model.children;
            for (var i = 0; i<children.length; i++) {
                var subModel = children[i];
                rom.$.parseModel(subModel, sf);
            }
        }
        // link is independed, so be created last.
        if ('links' in model) {
            var links = model.links;
            for (name in links) {
                rom.$.parseLink(name, links[name], sf);
            }
        }
        return sf;
    },

    loadModelFromJSON : function(modelContent) {
        var modelJson = JSON.parse(modelContent);
        rom.model = rom.$.parseModel(modelJson, null);
    },

    loadScriptFile : function(scriptFile) {
        process.load(scriptFile);
    },

    /*
     * get simfuns which have application
     */
    simfunsHaveApplication : function() {
        var haveAppSimfuns=[];
        var sfs = rom.library.simfuns;
        for (var sf in sfs) {
            if ('haveApp' in sfs[sf] && sfs[sf].haveApp)
                haveAppSimfuns.push([sf, sfs[sf].group, sfs[sf].name, sfs[sf].description]);
        }
        return haveAppSimfuns;
    },

    formatJavaScriptCode : function(code) {
        return require('beautify').js_beautify(code);
    },

    exportJSONFile : function(filename) {
        var f = new File(filename);
        if (!f.open(File.WriteOnly | File.Text))
            throw 'open file error.';
        var ts1 = new TextStream(f);
        ts1.writeString(require('beautify').js_beautify('{' + model.toJSON() + '}'));
        f.close();
    },

    /*
      从文件参数 filePar 中根据参数的json表示创建相应类型的参数.
      */
    createParameterByJSON : function(filePar, json) {
        var parObj = JSON.parse(json);
        //        log(filePar.name);
        //        log(json);
        var parameterClass = parObj.type;
        var factory = rom.library.parameters[parameterClass].factory;
        if (factory) {
            var grp = parObj.group && filePar.parameters[parObj.group];
            parObj.parameterClass = parObj.type;
            parObj.type = (filePar.mode === rom.InputFile ? rom.Input : rom.Output);
            parObj.description = parObj.desc;
            delete parObj.desc;
            if (!grp && parObj.group) {
                grp = new rom.GroupParameter(parObj.group, filePar);
                grp.canDelete = false;
            }
            delete parObj.group;

            var newParObj = null;
            newParObj = factory(parObj.id, parObj, grp || filePar, newParObj);
            return newParObj;
        }
        return null;
    },

    /*
     * clone simfun by simfun's json string
     */
    cloneSimfunByJSON : function(json) {
        var sf = JSON.parse(json);
        //clearAllLinksInSimfun(sf);
        return rom.$.parseModel(sf, null);
    },

    createSimfunByClassName : function(sfName, sfClassName, parentSf, index, newSequence, top, srName) {
        if (newSequence) {
            rom.enterBatchProcessStatus(true);
            var sr = new rom.Sequence(srName);
            var oldSf = parentSf.replace(index, sr);
            var newSf = new rom[sfClassName](sfName);
            if (top) {
                sr.append(newSf);
                sr.append(oldSf);
            } else {
                sr.append(oldSf);
                sr.append(newSf);
            }
            rom.enterBatchProcessStatus(false);
            return newSf;
        } else {
            var sf = new rom[sfClassName](sfName, null);
            if (parentSf)
                parentSf.insert(index, sf);
            return sf;
        }
    },

    callCustomMenuAction : function(sfClassName, menuItem, sf) {
        var sfmetadata = rom.library.simfuns[sfClassName];
        if ('tools' in sfmetadata) {
            var tools = sfmetadata.tools;
            for (var i = 0, sz = tools.length; i < sz; i++) {
                var tool = tools[i];
                if (tool.menuItem === menuItem) {
                    tool.actor(sf);
                    return;
                }
            }
        }
    }
}

function clearAllLinksInGroupParameter(groupVar) {
    var childVars = groupVar.children;
    for (var name in childVars) {
        if (childVars[name].parameterClass === 'GroupParameter')
            clearAllLinksInGroupParameter(childVars[name].parameters);
        else if (childVars[name].type === rom.Linked)
            childVars[name].type = rom.Input;
    }
}

function clearAllLinksInSimfun(sf) {
    if ('links' in sf) {
        delete sf.links;
    }
    if ('parameters' in sf) {
        var parameters = sf.parameters;
        for (var name in parameters) {
            if (parameters[name].parameterClass === 'GroupParameter') {
                clearAllLinksInGroupParameter(parameters[name]);
            } else if (parameters[name].type === rom.Linked) {
                parameters[name].type = rom.Input;
            }
        }
    }
    if ('children' in sf) {
        var children = sf.children;
        for (var i = 0; i < children.length; i++) {
            var subSf = children[i];
            clearAllLinksInSimfun(subSf);
        }
    }
}


/*
 * clone parameter by parameter's json string
 */
rom.cloneParameterByJSON = function(json) {
    var par = JSON.parse(json);
    for (var name in par) {
        return rom.$.parseParameter(name, par[name], null, null); // only one parameter
    }
}
