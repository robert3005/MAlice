class RedBlackTree
	constructor: ->
		header = new RedBlackNode
		header.left = header.right = nullNode

	compare: (item, t) ->
		if t is header
			1
		else
			item.compareTo t.element
	
	insert: (item) ->
		current = parent = grand = header
		nullNode.element = item
		while compare item current isnt 0
			great = grand
			grand = parent
			parent = current
			current = if compare item current < 0 then current.left else current.right
			if current.left.color is RED and current.right.color is RED
				handReorient item
		if current isnt nullNode
			throw new DuplicateItemException item.toString
		current = new RedBlackNode item nullNode nullNode
		if compare item parent < 0
			parent.left = current
		else
			parent.right = current
		handleReorient item

	remove: (x) ->
		throw new UnsupportedOperationException

	findMin: ->
		if isEmpty
			null
		itr = header.right
		while itr.left isnt nullNode
			itr = itr.left
		itr.element
	
	findMax: (x) ->
		nullNode.element = x
		current = header.right
		while true		
			if x.compareTo current.element < 0
				current = current.left
			else 
				if x.compareTo current.element > 0
					current = current.right
				else
					if current isnt nullNode
						current.element
					else
						null

	makeEmpty: ->
		header.right = nullNode
	
	printTree: ->
		printTree header.right
	
	printTree: (t) ->
		if t isnt nullNode
			printTree t.left
			print t.element
			printTree t.right

	isEmpty: ->
		header.right is nullNode
	
	handleReorient: (item) ->
		current.color = RED
		current.left.color = BLACK
		current.right.color = BLACK

		if parent.color is RED
			grand.color = RED
			if compare item grand < 0 isnt compare item parent < 0
				parent = rotate item grand
			current = rotate item great
			current.color = BLACK
		header.right.color = BLACK
	
	rotate: (item, parent) ->
		if compare item parent < 0
			if parent.left = compare item parent.left < 0 then rotateWithLeftChild parent.left
			else
				rotateWithRightChild parent.left
		else
			if parent.right = compare item parent.right < 0 then rotateWithLeftChild parent.right
			else
				rotateWithRightChild parent.right
	
	rotateWithLeftChild: (k2) ->
		k1 = k2.left
		k2.left = k1.right
		k1.right = k2
		k1			
	
	rotateWithRightChild: (k1) ->
		k2 = k1.right
		k1.right = k2.left
		k2.left = k1
		k2

class RedBlackNode
	constructor: (@theElement, @lt, @rt) ->
		color = RedBlackTree.BLACK

main = (a) ->
	t = new RedBlackTree
	t.nullNode = new RedBlackNode
	t.nullNode.right = t.nullNode.left = t.nullNode
	t.BLACK = 1
	t.RED = 0
	t.current = null
	t.parrent = null
	t.grand = null
	t.great = null
	NUMS = 4000000
	GAP = 35461
	i = GAP
	while i isnt 0
		t.insert i
		i = i + GAP % NUMS
	if t.findMin.intValue isnt 1 or t.findMax.intValue isnt NUMS - 1
		print "Error"
	i = 1
	while i < NUMS
		if t.find(i).intValue isnt i
			print "Error"
		i += 1
	print t
	console.log "Hello"


class DuplicateItemException
	constructor: (message) ->


console.log "Hello"
main null