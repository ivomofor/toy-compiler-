module {
  func.func @invalid_result(
      %lhs: tensor<2x3xf32>,
      %rhs: tensor<3x4xf32>
  ) -> tensor<2x5xf32> {
    %result = "toy.tensor.matmul"(%lhs, %rhs)
        : (tensor<2x3xf32>, tensor<3x4xf32>) -> tensor<2x5xf32>
    return %result : tensor<2x5xf32>
  }
}