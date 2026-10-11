module {
  func.func @invalid_rank(
      %lhs: tensor<2x3xf32>,
      %rhs: tensor<3xf32>
  ) -> tensor<2x3xf32> {
    %result = "toy.tensor.matmul"(%lhs, %rhs)
        : (tensor<2x3xf32>, tensor<3xf32>) -> tensor<2x3xf32>
    return %result : tensor<2x3xf32>
  }
}