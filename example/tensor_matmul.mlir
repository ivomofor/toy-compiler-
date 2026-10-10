module {
  func.func @matmul(
      %lhs: tensor<2x2xf32>,
      %rhs: tensor<2x2xf32>
  ) -> tensor<2x2xf32> {
    %result = "toy.tensor.matmul"(%lhs, %rhs)
        : (tensor<2x2xf32>, tensor<2x2xf32>) -> tensor<2x2xf32>
    return %result : tensor<2x2xf32>
  }
}
