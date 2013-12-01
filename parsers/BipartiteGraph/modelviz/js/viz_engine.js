/*
 * only functions and prototypes here
 */
function assert(condition, message) { if (!condition) { throw message || "Assertion failed"; }};
function contains (arr,e) { return (arr.indexOf(e) !== -1) ; };
function notcontains(arr,e) { return (! contains(arr,e)) ;} ;
function find(arr,e) { if (contains(arr,e)) { return arr.indexOf(e); } else { return false; } };
function checkAndAdd(arr,e) { if (notcontains(arr,e)) { arr.push(e); }; return true; }
function hasSubtypes(e) { return e.subtype !== undefined;}
function addEdge(edgelist,source,target,type,subtype)
{
    var condition = (source !== undefined) && (target !== undefined);
    if (condition)
        {
            if (subtype !== undefined) { edgelist.push({"source":source,"target":target,"type":type,"subtype":subtype} ); }
            else { edgelist.push({"source":source,"target":target,"type":type} ); }
        }
}

function deepcopy(obj) 
{ 
    var temp = JSON.parse(JSON.stringify(obj));
    temp.__proto__ = Object.getPrototypeOf(obj);
    return temp ; 
}
function keys (obj) 
{
    var keys=[];
    for (var key in obj) { keys.push(key); };
    return keys;
}
function vals (obj)
{
    var vals=[];
    for (var key in obj) { vals.push(obj[key]); };
    return vals;
}
function instanceOf(object, constructor) 
{
    while (object !== null) 
    {
        if (object === constructor.prototype) return true;
        object = Object.getPrototypeOf(object);
    }
    return false;
}
function nodeEquals(node1,node2) {return (node1.type===node2.type && node1.idx===node2.idx); }
function uniqueNodes(arr)
{
    var a = arr;
    for(var i=0; i<a.length; ++i) 
        {
            for(var j=i+1; j<a.length; ++j) 
            {
                if(nodeEquals(a[i],a[j])) a.splice(j--, 1);
            }
        }
    return a;
}
function unique(arr)
{
    var a = arr;
    for(var i=0; i<a.length; ++i) 
        {
            for(var j=i+1; j<a.length; ++j) 
            {
                if((a[i]===a[j])) a.splice(j--, 1);
            }
        }
    return a;
}


function clone(obj) 
{
    //
    //console.log(obj.constructor)
    switch (obj.constructor)
    {
        //case Object: { var t = {}; var keylist = keys(obj); for (var i in keylist)  { t[keylist[i]] = clone(obj[i]); } return t; }
        case Object: return JSON.parse(JSON.stringify(obj));
        case Array: return obj.map(clone);
        case Boolean: return obj;
        case Number: return obj;
        case String: return obj;
        case undefined: return obj;
        default: return new obj.constructor(obj);
    }
}

function NodeSelector(nodes1) 
{ 
    // types = {type1:true, type2:true}
    // subtypes = {type1: {subtype1:true, subtype2:true}}
    var makeTypes = function(nodes)
    {
        var typeslist = unique(nodes.map(function (e) {return e.type; }));
        var types = {};
        typeslist.forEach(function (e) {types[e]=true;} );
        return types;
    };
    var makeSubtypes = function(nodes)
    {     
        var subtypeslist = unique(nodes.map( function (e) { return e.subtype ? (e.type + " " + e.subtype):"";} ));
        var subtypes = {};
        for (var i in subtypeslist)
        {
            if (subtypeslist[i])
            {
                var temp = subtypeslist[i].split(" ");
                if ( notcontains(keys(subtypes),temp[0])) {subtypes[temp[0]]= {};};
                subtypes[temp[0]][temp[1]] = true;
            }
        }
        return subtypes;        
    };
    
    this.types = instanceOf(nodes1,NodeSelector) ? clone(nodes1.types) : makeTypes(nodes1);
    this.subtypes = instanceOf(nodes1,NodeSelector) ? clone(nodes1.subtypes) : makeSubtypes(nodes1);
    
    this.filter = function(nodes)
    {
        var types = this.types;
        var subtypes = this.subtypes;
        
        return nodes
                .filter(function(e) { return types[e.type]; })
                .filter(function(e) { return ( (!hasSubtypes(e)) || subtypes[e.type][e.subtype] ); });           
    };
    
    this.makeFalse = function()
    {
        for (var i in this.types) { this.types[i] = false; }
        for (var i in this.subtypes) { for (var j in this.subtypes[i]) { this.subtypes[i][j] = false; }}
    };
}


function NodeSet(nodeselector)
{
    var makeTypes = function(nodeselector)
    {
        var types = {};
        for (var i in nodeselector.types) { types[i] = []; }
        return types;
    };
    
    
    var makeSubtypes = function(nodeselector)
    {
        var subtypes = {};       
        for (var i in nodeselector.subtypes) 
        { 
            subtypes[i] = {}; 
            for (var j in nodeselector.subtypes[i]) { subtypes[i][j] = []; }
        }
        return subtypes;
    };
    
    this.types = instanceOf(nodeselector,NodeSet) ? clone(nodeselector.types): makeTypes(nodeselector);
    this.subtypes = instanceOf(nodeselector,NodeSet) ? clone(nodeselector.subtypes) : makeSubtypes(nodeselector);
    
    this.addNodes = function( nodes )
    {
        for (var i in nodes) 
        {
            checkAndAdd(this.types[nodes[i].type],nodes[i].idx); 
            if (hasSubtypes(nodes[i])) { checkAndAdd(this.subtypes[nodes[i].type][nodes[i].subtype],nodes[i].idx); }
        }
    };
    
    
    this.deleteNodes = function( nodes )
    {
        for (var i in nodes)
        {
             var node = nodes[i];
             var ind = find(this.types[node.type],node.idx);
             if (ind !== false) { this.types[node.type].splice(ind,1); }
             if (hasSubtypes(node))
             {
                 var ind = find(this.subtypes[node.type][node.subtype],node.idx);
                 if (ind !== false) { this.subtypes[node.type][node.subtype].splice(ind,1); }
             }
         }
    };
    
    this.empty = function ()
    {
        for (var i in this.types) { this.types[i] = []; }
        for (var i in this.subtypes)
        {
            for (var j in this.subtypes[i]) { this.subtypes[i][j] = []; }
        }
    };
    
    this.filter = function(nodes) { return nodes.filter( function(e) { return contains(this.types[e.type],e.idx); }); };
}

function EdgeSelector (edges)
{
    var makeEdgeSelector = function(edges)
    {
        var temp = {"types":{}, "subtypes":{}};
        for (var i in edges)
        {
            if (edges[i].constructor===Array) {temp.types[i]= true;}
            else 
            {
                temp.subtypes[i] = {};
                for (var j in edges[i]) { temp.subtypes[i][j] = true;}
            }
        }
        return temp;
    };
    this.types = instanceOf(edges,EdgeSelector)? clone(edges.types) : makeEdgeSelector(edges).types;
    this.subtypes = instanceOf(edges,EdgeSelector)? clone(edges.subtypes) : makeEdgeSelector(edges).subtypes;
    
    this.makeFalse = function ()
    {
        for (var i in this.types) { this.types[i] = false; }
        for (var i in this.subtypes) { for (var j in this.subtypes[i]) { this.subtypes[i][j] = false; }} 
    };    
}

function EdgeSet (edges)
{
    var makeEdgeSet =function (edges)
    {
        var temp = {"types":{}, "subtypes":{}};
        for (var i in edges)
        {
            if (edges[i].constructor===Array) {temp.types[i]= clone(edges[i]);}
            else 
            {
                temp.subtypes[i] = {};
                for (var j in edges[i]) { temp.subtypes[i][j] = clone(edges[i][j]);}
            }
        }
        return temp;
    };
    
    this.types = instanceOf(edges,EdgeSet) ? clone(edges.types) : makeEdgeSet(edges).types;
    this.subtypes = instanceOf(edges,EdgeSet) ? clone(edges.subtypes) : makeEdgeSet(edges).subtypes;
    
}

function generateEdges(nodes,edgeselector,edgeset)
{
    var edgelist = [];
    
    //r2t
    if (edgeselector.types.r2t) 
    {
        var type = "r2t";
        edgeset.types.r2t.forEach( function(e) 
        {
            var r = nodes.filter( function(o) {return (o.idx===e[0]) && (o.type==="r"); })[0];
            var t = nodes.filter( function(o) {return (o.idx===e[1]) && (o.type==="t"); })[0];
            addEdge(edgelist,r,t,type);
        });
    }
    
    //r2p
    for (var subtype in edgeselector.subtypes.r2p) 
    {
        var type = "r2p";
        if (edgeselector.subtypes.r2p[subtype])
        {
            edgeset.subtypes.r2p[subtype].forEach( function(e) 
            {
                var r = nodes.filter( function(o) {return (o.idx===e[0]) && (o.type==="r"); })[0];
                var p = nodes.filter( function(o) {return (o.idx===e[1]) && (o.type==="p"); })[0];
                //addEdge order will depend on subtype of r2p: 
                //context: p->r, delcontext p->r, syncontext r->p
                if (subtype==='syncontext') {  addEdge(edgelist,r,p,type,subtype); }
                else {  addEdge(edgelist,p,r,type,subtype); }
            });
        }
    }
    
    //t2p
    for (var subtype in edgeselector.subtypes.t2p) 
    {
        var type = "t2p";
        if (edgeselector.subtypes.t2p[subtype])
        {
            edgeset.subtypes.t2p[subtype].forEach( function(e) 
            {
                var t = nodes.filter( function(o) {return (o.idx===e[0]) && (o.type==="t"); })[0];
                var p = nodes.filter( function(o) {return (o.idx===e[1]) && (o.type==="p"); })[0];
                //addEdge order will depend on subtype of t2p:
                // reactant: p->t, product: t->p, context: p->t, delcontext p->t, syncontext t->p
                if (subtype==='syncontext' || subtype==='product') {  addEdge(edgelist,t,p,type,subtype); }
                else {  addEdge(edgelist,p,t,type,subtype); }
            });
        }
    }
    
    //tp2t
    for (var subtype in edgeselector.subtypes.tp2t) 
    {
        var type = "tp2t";
        if (edgeselector.subtypes.tp2t[subtype])
        {
            edgeset.subtypes.tp2t[subtype].forEach( function(e) 
            {
                var tp = nodes.filter( function(o) {return (o.idx===e[0]) && (o.type==="tp"); })[0];
                var t = nodes.filter( function(o) {return (o.idx===e[1]) && (o.type==="t"); })[0];
                //addEdge order will depend on subtype of tp2t:
                // forward is the nominal direction and can be said to be enhanced
                // therefore forward: tp->t, reverse: t->tp
                if (subtype==='forward') {  addEdge(edgelist,tp,t,type,subtype); }
                else {  addEdge(edgelist,t,tp,type,subtype); }
            });
        }
    }
    
    //tp2p
    for (var subtype in edgeselector.subtypes.tp2p) 
    {
        var type = "tp2p";
        if (edgeselector.subtypes.tp2p[subtype])
        {
            edgeset.subtypes.tp2p[subtype].forEach( function(e) 
            {
                var tp = nodes.filter( function(o) {return (o.idx===e[0]) && (o.type==="tp"); })[0];
                var p = nodes.filter( function(o) {return (o.idx===e[1]) && (o.type==="p"); })[0];
                //addEdge order will depend on subtype of tp2t:
                // forwardcontext: p->tp, reversecontext: p->tp, 
                //forwardreactant: p->tp, reversereactant: tp->p
                //syncontext: tp->p, delcontext: p->tp
                if (subtype==='reversereactant' || subtype==='syncontext') {  addEdge(edgelist,tp,p,type,subtype); }
                else {  addEdge(edgelist,p,tp,type,subtype); }
            });
        }
    }
    
    return edgelist;
}

/*
a = new EdgeSelector(data.edges);
b = new EdgeSet(data.edges);
c = generateEdges(data.nodes,a,b);
n = data.nodes;
e = b.types.r2t[0];
*/

