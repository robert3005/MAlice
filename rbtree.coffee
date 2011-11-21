module.exports = (() ->
	BLACK = 1
	RED = 2

	class RBTNode
		constructor: (@key, @argumentsType) ->
			@color = null
			@left = null
			@right = null
			@parent = null

	class RBTree
		constructor: ->
			@nil = new RBTNode
			@nil.color = BLACK
			@root = @nil

		rbFind: (key) ->
			current = @root
			while current isnt @nil and key isnt current.key
				if key < current.key
					current = current.left
				else
					current = current.right
			if key isnt current.key then null else current

		rbInsert: (nodeIn) ->
			previous = @nil
			current = @root
			while current isnt @nil
				previous = current
				if nodeIn.key < current.key
					current = current.left
				else
					current = current.right
			nodeIn.parent = previous
			if previous is @nil
				@root = nodeIn
			else
				if nodeIn.key < previous.key
					previous.left = nodeIn
				else
					previous.right = nodeIn
			nodeIn.left = @nil
			nodeIn.right = @nil
			nodeIn.color = RED
			@rbInsertFixup nodeIn		

		leftRotate: (node1) ->
			node2 = node1.right
			node1.right = node2.left
			if node2.left isnt @nil
				node2.left.parent = node1
			node2.parent = node1.parent
			if node1.parent is @nil
				@root = node2
			else
				if node1 is node1.parent.left
					node1.parent.left = node2
				else
					node1.parent.right = node2
			node2.left = node1
			node1.parent = node2

		rightRotate: (node1) ->
			node2 = node1.left
			node1.left = node2.right
			if node2.right isnt @nil
				node2.right.parent = node1
			node2.parent = node1.parent
			if node1.parent is @nil
				@root = node2
			else
				if node1 is node1.parent.right
					node1.parent.right = node2
				else
					node1.parent.left = node2
			node2.right = node1
			node1.parent = node2
			
		rbInsertFixup: (node) ->
			while node.parent.color is RED
				if node.parent is node.parent.parent.left
					uncle = node.parent.parent.right
					if uncle?.color is RED
						node.parent.color = BLACK
						uncle.color = BLACK
						node.parent.parent.color = RED
						node = node.parent.parent
					else
						if node is node.parent.right
							node = node.parent
							@leftRotate node
						node.parent.color = BLACK
						node.parent.parent.color = RED
						@rightRotate node.parent.parent
				else
					uncle = node.parent.parent.left
					if uncle?.color is RED
						node.parent.color = BLACK
						uncle.color = BLACK
						node.parent.parent.color = RED
						node = node.parent.parent
					else
						if node is node.parent.left
							node = node.parent
							@rightRotate node
						node.parent.color = BLACK
						node.parent.parent.color = RED
						@leftRotate node.parent.parent
			@root.color = BLACK
	return [RBTree, RBTNode]
)()