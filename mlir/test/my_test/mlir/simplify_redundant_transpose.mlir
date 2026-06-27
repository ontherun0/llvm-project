module {
  func.func @test_unmatched_transpose_transpose(%arg0: vector<2x3x4xf32>) -> vector<4x2x3xf32> {
    %0 = vector.transpose %arg0, [1, 2, 0] : vector<2x3x4xf32> to vector<3x4x2xf32>
    %1 = vector.transpose %0, [1, 2, 0] : vector<3x4x2xf32> to vector<4x2x3xf32>
    return %1 : vector<4x2x3xf32>
  }

  func.func @test_matched_transpose_transpose(%arg0: vector<2x3x4xf32>) -> vector<2x3x4xf32> {
    %0 = vector.transpose %arg0, [1, 2, 0] : vector<2x3x4xf32> to vector<3x4x2xf32>
    %1 = vector.transpose %0, [2, 0, 1] : vector<3x4x2xf32> to vector<2x3x4xf32>
    return %1 : vector<2x3x4xf32>
  }
}
