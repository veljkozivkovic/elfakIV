import cv2
import numpy as np
import matplotlib.pyplot as plt


def apply_black_square(matrix, coords, size=4):
    half_size = size // 2
    for x, y in coords:
        matrix[x - half_size: x + half_size, y - half_size: y + half_size] = 0

def main():
    # Učitavanje slike u sivoj skali
    image = cv2.imread('slika_1.png', cv2.IMREAD_GRAYSCALE)

    # Primena Fourierove transformacije za prelazak u frekventni domen
    fourier_transform = np.fft.fft2(image)
    shifted_spectrum = np.fft.fftshift(fourier_transform)

    # Izračunavanje magnituda spektra
    magnitude_before = 20 * np.log(np.abs(shifted_spectrum) + 1)

    # Prikaz magnituda spektra pre filtriranja
    plt.imshow(magnitude_before)
    plt.title('Magnituda spektra pre filtriranja')
    plt.savefig('magnitude_before.png')
    plt.show()

    # Koordinate za uklanjanje periodičnog šuma
    noise_coords = [(266, 205), (246, 245), (266, 265), (246, 306)]
    apply_black_square(shifted_spectrum, noise_coords, size=5)

    # Prikaz magnituda spektra nakon filtriranja
    magnitude_after = 20 * np.log(np.abs(shifted_spectrum) + 1)
    plt.imshow(magnitude_after)
    plt.title('Magnituda spektra nakon filtriranja')
    plt.savefig('magnitude_after.png')
    plt.show()

    # Inverzna Fourierova transformacija
    inverse_shifted = np.fft.ifftshift(shifted_spectrum)
    filtered_image = np.fft.ifft2(inverse_shifted).real

    # Prikaz filtrirane slike
    plt.imshow(filtered_image, cmap='gray')
    plt.title('Filtrirana slika')
    plt.savefig('filtered_output.png')
    plt.show()

    # Čuvanje filtrirane slike
    cv2.imwrite('filtered_image.png', filtered_image.astype(np.uint8))

if __name__ == '__main__':
    main()
