module {
  func.func @invalid_matmul(
      %lhs: tensor<2x3xf32>,
      %rhs: tensor<2x4xf32>
  ) -> tensor<2x4xf32> {
    %result = "toy.tensor.matmul"(%lhs, %rhs)
        : (tensor<2x3xf32>, tensor<2x4xf32>) -> tensor<2x4xf32>
    return %result : tensor<2x4xf32>
  }
}