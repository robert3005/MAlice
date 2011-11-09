BLACK = 1
RED = 2

class RBNode
	constructor: ->
		@key = null
		@color = null
		@left = null
		@right = null
		@p = null


class RBTree
	constructor: ->
		@nil = new RBNode
		nil.color = BLACK
		@root = null

	rbFind: (T, k) ->
		x = T.root
		while x isnt T.nil and k isnt x.key
			if k < x.key
				x = x.left
			else
				x = x.right
		x
	
	leftRotate: (T, x) ->
		y = x.right
		x.right = y.left
		if y.left isnt T.nil
			y.left.p = x
		y.p = x.p
		if x.p is T.nil
			T.root = y
		else
			if x is x.p.left
				x.p.left = y
			else
				x.p.right = y
		y.left = x
		x.p = y

	rightRotate: (T, x) ->
		y = x.left
		x.left = y.right
		if y.right isnt T.nil
			y.right.p = x
		y.p = x.p
		if x.p is T.nil
			T.root = y
		else
			if x is x.p.right
				x.p.right = y
			else
				x.p.left = y
		y.right = x
		x.p = y

	rbInsert: (T, z) ->
		y = T.nil
		x = T.root
		while x isnt T.nil
			y = x
			if z.key < x.key
				x = x.left
			else
				x = x.right
		z.p = y
		if y is T.nil
			T.root = z
		else
			if z.key < y.key
				y.left = z
			else
				y.right = z
		z.left = T.nil
		z.right = T.nil
		z.color = RED
		rbInsertFixup T z
		
	rbInsertFixup: (T, z) ->
		while z.p.color is RED
			if z.p is z.p.p.left
				y = z.p.p.right
				if y.color is RED
					z.p.color = BLACK
					y.color = BLACK
					z.p.p.color = RED
					z = z.p.p
				else
					if z is z.p.right
						z = z.p
						leftRotate T z
					z.p.color = BLACK
					z.p.p.color = RED
					rigthRotate T z
			else
				y = z.p.p.left
				if y.color is RED
					z.p.color = BLACK
					y.color = BLACK
					z.p.p.color = RED
					z = z.p.p
				else
					if z is z.p.left
						z = z.p
						rightRotate T z
					z.p.color = BLACK
					z.p.p.color = RED
					leftRotate T z
		T.root.color = BLACK