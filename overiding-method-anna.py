import numpy as np
import math

class Calculator:
    def __init__(self, stack_size=100):
        self.stack = []
        self.stack_size = stack_size
        self.operations = {}
        self.register_default_operations()
    
    # Override push to enforce max stack size
    def push(self, value):
        if len(self.stack) >= self.stack_size:
            raise OverflowError(f"Stack overflow: max size {self.stack_size} reached.")
        self.stack.append(value)
    
    # Override pop with underflow check
    def pop(self):
        if not self.stack:
            raise IndexError("Stack underflow: stack is empty.")
        return self.stack.pop()
    
    def register_operation(self, name, func):
        self.operations[name] = func
    
    def execute(self, op_name, *args):
        if op_name not in self.operations:
            raise ValueError(f"Operation '{op_name}' not registered.")
        return self.operations[op_name](self, *args)
    
    def register_default_operations(self):
        # Binary ops use _binary_op helper, unary use _unary_op helper

        # Basic arithmetic
        self.register_operation('add', lambda self: self._binary_op(lambda a,b: b + a))
        self.register_operation('sub', lambda self: self._binary_op(lambda a,b: b - a))
        self.register_operation('mul', lambda self: self._binary_op(lambda a,b: b * a))
        self.register_operation('div', lambda self: self._binary_op(lambda a,b: b / a if a != 0 else float('inf')))
        
        # Scientific unary functions
        self.register_operation('sin', lambda self: self._unary_op(math.sin))
        self.register_operation('cos', lambda self: self._unary_op(math.cos))
        self.register_operation('tan', lambda self: self._unary_op(math.tan))
        self.register_operation('log', lambda self: self._unary_op(lambda x: math.log10(x) if x > 0 else float('nan')))
        self.register_operation('ln', lambda self: self._unary_op(lambda x: math.log(x) if x > 0 else float('nan')))
        self.register_operation('pow', lambda self: self._binary_op(lambda a,b: b ** a))
        self.register_operation('sqrt', lambda self: self._unary_op(lambda x: math.sqrt(x) if x >= 0 else float('nan')))
        self.register_operation('neg', lambda self: self._unary_op(lambda x: -x))

        # Matrix / Vector operations
        self.register_operation('mat_add', lambda self: self._binary_op(np.add))
        self.register_operation('mat_sub', lambda self: self._binary_op(np.subtract))
        self.register_operation('mat_mul', lambda self: self._binary_op(np.matmul))
        self.register_operation('mat_transpose', lambda self: self._unary_op(np.transpose))
        self.register_operation('mat_det', lambda self: self._unary_op(np.linalg.det))
        self.register_operation('mat_inv', lambda self: self._unary_op(np.linalg.inv))
        self.register_operation('vec_dot', lambda self: self._binary_op(np.dot))
        self.register_operation('vec_cross', lambda self: self._binary_op(np.cross))

    # Binary operation helper: pops two values, applies func, pushes result
    def _binary_op(self, func):
        a = self.pop()
        b = self.pop()
        result = func(a, b)
        self.push(result)
    
    # Unary operation helper: pops one value, applies func, pushes result
    def _unary_op(self, func):
        a = self.pop()
        result = func(a)
        self.push(result)
    
    def __str__(self):
        # Show stack nicely, limit display for large stacks
        if len(self.stack) > 20:
            displayed = ', '.join(map(str, self.stack[-10:]))
            return f"Stack (last 10 of {len(self.stack)}): [{displayed}]"
        else:
            return f"Stack: {self.stack}"

# Example usage:
if __name__ == "__main__":
    calc = Calculator()

    # Push 100 numbers to test overflow protection
    try:
        for i in range(100):
            calc.push(i + 1)
        print(calc)
        # Pushing one more should raise OverflowError
        calc.push(101)
    except OverflowError as e:
        print(e)

    # Test basic arithmetic
    calc.push(5)
    calc.push(3)
    calc.execute('add')
    print(calc)  # Should show 8 on top

    # Test scientific function
    calc.push(math.pi / 2)
    calc.execute('sin')
    print(calc)  # Should show 1

    # Test matrix operations
    calc.push(np.array([[1, 2], [3, 4]]))
    calc.execute('mat_det')
    print(calc)  # Should show determinant -2.0

