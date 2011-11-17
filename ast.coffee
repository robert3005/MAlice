
class Node
	constructor: ( @type, args ) ->
		return
			switch @type
				when NODE_OP then new OPNode args
				when NODE_VAR then new VARNode args
				when NODE_CONST then new CONSTNode args
				when NODE_TYPE then new TYPENode args

class OPNode extends Node
	constructor: ( @value, children ) ->

class VARNode extends Node
	constructor: ( @value ) ->

class CONSTNode extends Node
	constructor: ( @value ) ->

class TYPENode extends Node
	constructor: ( @value, args ) ->