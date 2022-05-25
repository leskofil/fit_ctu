import numpy as np

def add_padding(image, padding):
    n, m = image.shape

    padded_matrix = np.zeros((n + padding * 2, m + padding * 2))
    padded_matrix[padding : n + padding, padding : m + padding] = image

    return padded_matrix

def clamp(n, minn, maxn):
    return min(max(n, minn), maxn)



def conv2d_fast(img, krn):
    import numpy as np
    
    is0, is1 = img.shape
    ks0, ks1 = krn.shape
    rs0, rs1 = is0 - ks0 + 1, is1 - ks1 + 1
    
    ix0 = np.arange(ks0)[:, None] + np.arange(rs0)[None, :]
    ix1 = np.arange(ks1)[:, None] + np.arange(rs1)[None, :]
    
    res = krn[:, None, :, None] * img[(ix0.ravel()[:, None], ix1.ravel()[None, :])].reshape(ks0, rs0, ks1, rs1)
    res = res.transpose(1, 3, 0, 2).reshape(rs0, rs1, -1).sum(axis = -1)
    
    return res

def conv2d_new(img, krn):
    import numpy as np

    is0, is1 = img.shape
    ks0, ks1 = krn.shape
    rs0, rs1 = is0 - ks0 + 1, is1 - ks1 + 1
    res = np.zeros((rs0, rs1))
    
    for k in range(ks0):
        for l in range(ks1):
            res[...] += krn[k, l] * img[k : k + rs0, l : l + rs1]
    res = np.clip(res, 0, 255).astype(np.uint8)
            
    return res

def conv2d(image: np.array, kernel: np.array, padding: int) -> np.array:
    k = kernel.shape
    n, m = image.shape

    image = add_padding(image, padding)

    h_out = np.floor((n + 2 * padding - k[0])).astype(int) + 1
    w_out = np.floor((m + 2 * padding - k[1])).astype(int) + 1

    matrix_out = np.zeros((h_out, w_out))

    ms = matrix_out.shape
    res = conv2d_new(image, kernel)

    # b = k[0] // 2, k[1] // 2
    # center_x_0 = b[0]
    # center_y_0 = b[1]
    # for i in range(h_out):
        # center_x = center_x_0 + i 
        # indices_x = [center_x + l for l in range(-b[0], b[0] + 1)]
        # for j in range(w_out):
            # center_y = center_y_0 + j
            # indices_y = [center_y + l for l in range(-b[1], b[1] + 1)]
# 
            # submatrix = image[indices_x, :][:, indices_y]
# 
            # matrix_out[i][j] = clamp(np.sum(np.multiply(submatrix, kernel)), 0, 255)
    return res


def repair_kernel(kernel):
    new_kernel = np.zeros((kernel.shape[0] + 1, kernel.shape[1] + 1))
    new_kernel[:kernel.shape[0], :kernel.shape[1]] = kernel
    return new_kernel

def apply_filter(image: np.array, kernel: np.array) -> np.array:
    # A given image has to have either 2 (grayscale) or 3 (RGB) dimensions
    assert image.ndim in [2, 3]
    # A given filter has to be 2 dimensional and square
    assert kernel.ndim == 2
    assert kernel.shape[0] == kernel.shape[1]

    if (kernel.shape[0] % 2 == 0):
        kernel = repair_kernel(kernel)
    kernel_shape = kernel.shape
    img_shape = image.shape
    
    if (image.ndim == 3):           
        result = np.dstack([conv2d(image[:, :, z], kernel, kernel_shape[0]//2) 
                           for z in range(3)]).astype('uint8')
    else:
        result = conv2d(image, kernel, kernel_shape[0]//2).astype('uint8')
    return result

