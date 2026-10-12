module {
  func.func @matmul_2x2(
      %lhs: tensor<2x2xf32>,
      %rhs: tensor<2x2xf32>
  ) -> tensor<2x2xf32> {
    %result = "toy.tensor.matmul"(%lhs, %rhs)
        : (tensor<2x2xf32>, tensor<2x2xf32>) -> tensor<2x2xf32>
    return %result : tensor<2x2xf32>
  }

  func.func @matmul_2x3_by_3x4(
      %lhs: tensor<2x3xf32>,
      %rhs: tensor<3x4xf32>
  ) -> tensor<2x4xf32> {
    %result = "toy.tensor.matmul"(%lhs, %rhs)
        : (tensor<2x3xf32>, tensor<3x4xf32>) -> tensor<2x4xf32>
    return %result : tensor<2x4xf32>
  }

  func.func @matmul_4x2_by_2x3(
      %lhs: tensor<4x2xf32>,
      %rhs: tensor<2x3xf32>
  ) -> tensor<4x3xf32> {
    %result = "toy.tensor.matmul"(%lhs, %rhs)
        : (tensor<4x2xf32>, tensor<2x3xf32>) -> tensor<4x3xf32>
    return %result : tensor<4x3xf32>
  }

  func.func @matmul_dynamic(
      %lhs: tensor<?x3xf32>,
      %rhs: tensor<3x?xf32>
  ) -> tensor<?x?xf32> {
      %result = "toy.tensor.matmul"(%lhs, %rhs)
          : (tensor<?x3xf32>, tensor<3x?xf32>) -> tensor<?x?xf32>
      return %result : tensor<?x?xf32>
  }
}

