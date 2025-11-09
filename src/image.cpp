#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

#include "wak/image.h"
#include "wak/parallel_for.h"

namespace wak
{

Image1 ReadImage1(
    const std::filesystem::path& filename, int32 channel, bool non_color, std::function<Image1::Type(Image1::Type)> transform
)
{
    stbi_set_flip_vertically_on_load(true);
    stbi_ldr_to_hdr_gamma(non_color ? 1.0f : 2.2f);

    int32 width, height;
    int32 components_per_pixel;
    float* data = stbi_loadf(filename.string().c_str(), &width, &height, &components_per_pixel, STBI_rgb_alpha);

    if (!data || channel < 0 || channel >= components_per_pixel)
    {
        std::cerr << "Failed to read image1: " << filename.string().c_str() << std::endl;
        return {};
    }

    constexpr int32 stride = STBI_rgb_alpha;
    Image1 image(width, height);

    if (channel < 3)
    {
        if (width * height > 64 * 1024)
        {
            if (transform)
            {
                ParallelFor(0, width * height, [&](int32 i) {
                    image[i] = transform(Float(std::fmax(0, data[stride * i + channel])));
                });
            }
            else
            {
                ParallelFor(0, width * height, [&](int32 i) { image[i] = Float(std::fmax(0, data[stride * i + channel])); });
            }
        }
        else
        {
            if (transform)
            {
                for (int32 i = 0; i < width * height; ++i)
                {
                    image[i] = transform(Float(std::fmax(0, data[stride * i + channel])));
                }
            }
            else
            {
                for (int32 i = 0; i < width * height; ++i)
                {
                    image[i] = Float(std::fmax(0, data[stride * i + channel]));
                }
            }
        }
    }
    else if (components_per_pixel == STBI_rgb_alpha)
    {
        if (transform)
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] = transform(Float(std::fmax(0, data[stride * i + channel])));
            }
        }
        else
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] = Float(std::fmax(0, data[stride * i + channel]));
            }
        }
    }

    stbi_image_free(data);
    return image;
}

Image3 ReadImage3(const std::filesystem::path& filename, bool non_color, std::function<Image3::Type(Image3::Type)> transform)
{
    stbi_set_flip_vertically_on_load(true);
    stbi_ldr_to_hdr_gamma(non_color ? 1.0f : 2.2f);

    int32 width, height;
    int32 components_per_pixel;
    float* data = stbi_loadf(filename.string().c_str(), &width, &height, &components_per_pixel, STBI_rgb);

    if (!data)
    {
        std::cerr << "Failed to read image3: " << filename.string().c_str() << std::endl;
        return {};
    }

    constexpr int32 stride = STBI_rgb;
    Image3 image(width, height);

    if (width * height > 64 * 1024)
    {
        if (transform)
        {
            ParallelFor(0, width * height, [&](int32 i) {
                image[i] = transform(Max(Vec3{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2] }, Vec3::zero));
            });
        }
        else
        {
            ParallelFor(0, width * height, [&](int32 i) {
                image[i] = Max(Vec3{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2] }, Vec3::zero);
            });
        }
    }
    else
    {
        if (transform)
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] = transform(Max(Vec3{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2] }, Vec3::zero));
            }
        }
        else
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] = Max(Vec3{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2] }, Vec3::zero);
            }
        }
    }

    stbi_image_free(data);
    return image;
}

Image4 ReadImage4(const std::filesystem::path& filename, bool non_color, std::function<Image4::Type(Image4::Type)> transform)
{
    stbi_set_flip_vertically_on_load(true);
    stbi_ldr_to_hdr_gamma(non_color ? 1.0f : 2.2f);

    int32 width, height;
    int32 components_per_pixel;
    float* data = stbi_loadf(filename.string().c_str(), &width, &height, &components_per_pixel, STBI_rgb_alpha);

    if (!data)
    {
        std::cerr << "Failed to read image4: " << filename.string().c_str() << std::endl;
        return {};
    }

    constexpr int32 stride = STBI_rgb_alpha;
    Image4 image(width, height);

    if (width * height > 64 * 1024)
    {
        if (transform)
        {
            ParallelFor(0, width * height, [&](int32 i) {
                image[i] = transform(
                    Max(Vec4{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2],
                              (components_per_pixel == STBI_rgb_alpha ? data[stride * i + 3] : 1) },
                        Vec4::zero)
                );
            });
        }
        else
        {
            ParallelFor(0, width * height, [&](int32 i) {
                image[i] =
                    Max(Vec4{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2],
                              (components_per_pixel == STBI_rgb_alpha ? data[stride * i + 3] : 1) },
                        Vec4::zero);
            });
        }
    }
    else
    {
        if (transform)
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] = transform(
                    Max(Vec4{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2],
                              (components_per_pixel == STBI_rgb_alpha ? data[stride * i + 3] : 1) },
                        Vec4::zero)
                );
            }
        }
        else
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] =
                    Max(Vec4{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2],
                              (components_per_pixel == STBI_rgb_alpha ? data[stride * i + 3] : 1) },
                        Vec4::zero);
            }
        }
    }

    stbi_image_free(data);
    return image;
}

Image1u ReadImage1u(
    const std::filesystem::path& filename, int32 channel, bool non_color, std::function<Image1u::Type(Image1u::Type)> transform
)
{
    stbi_set_flip_vertically_on_load(true);
    stbi_ldr_to_hdr_gamma(non_color ? 1.0f : 2.2f);

    int32 width, height;
    int32 components_per_pixel;
    uint8* data = stbi_load(filename.string().c_str(), &width, &height, &components_per_pixel, STBI_rgb_alpha);

    if (!data || channel < 0 || channel >= components_per_pixel)
    {
        std::cerr << "Failed to read image1: " << filename.string().c_str() << std::endl;
        return {};
    }

    constexpr int32 stride = STBI_rgb_alpha;
    Image1u image(width, height);

    if (channel < 3)
    {
        if (width * height > 64 * 1024)
        {
            if (transform)
            {
                ParallelFor(0, width * height, [&](int32 i) {
                    image[i] = transform(std::max<uint8>(uint8(0), data[stride * i + channel]));
                });
            }
            else
            {
                ParallelFor(0, width * height, [&](int32 i) {
                    image[i] = std::max<uint8>(uint8(0), data[stride * i + channel]);
                });
            }
        }
        else
        {
            if (transform)
            {
                for (int32 i = 0; i < width * height; ++i)
                {
                    image[i] = transform(std::max<uint8>(uint8(0), data[stride * i + channel]));
                }
            }
            else
            {
                for (int32 i = 0; i < width * height; ++i)
                {
                    image[i] = std::max<uint8>(uint8(0), data[stride * i + channel]);
                }
            }
        }
    }
    else if (components_per_pixel == STBI_rgb_alpha)
    {
        if (transform)
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] = transform(Float(std::max<uint8>(uint8(0), data[stride * i + channel])));
            }
        }
        else
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] = Float(std::max<uint8>(uint8(0), data[stride * i + channel]));
            }
        }
    }

    stbi_image_free(data);
    return image;
}

Image3u ReadImage3u(const std::filesystem::path& filename, bool non_color, std::function<Image3u::Type(Image3u::Type)> transform)
{
    stbi_set_flip_vertically_on_load(true);
    stbi_ldr_to_hdr_gamma(non_color ? 1.0f : 2.2f);

    int32 width, height;
    int32 components_per_pixel;
    uint8* data = stbi_load(filename.string().c_str(), &width, &height, &components_per_pixel, STBI_rgb);

    if (!data)
    {
        std::cerr << "Failed to read image3: " << filename.string().c_str() << std::endl;
        return {};
    }

    constexpr int32 stride = STBI_rgb;
    Image3u image(width, height);

    if (width * height > 64 * 1024)
    {
        if (transform)
        {
            ParallelFor(0, width * height, [&](int32 i) {
                image[i] = transform(
                    Max(Vector3<uint8>{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2] }, Vector3<uint8>::zero)
                );
            });
        }
        else
        {
            ParallelFor(0, width * height, [&](int32 i) {
                image[i] =
                    Max(Vector3<uint8>{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2] }, Vector3<uint8>::zero);
            });
        }
    }
    else
    {
        if (transform)
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] = transform(
                    Max(Vector3<uint8>{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2] }, Vector3<uint8>::zero)
                );
            }
        }
        else
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] =
                    Max(Vector3<uint8>{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2] }, Vector3<uint8>::zero);
            }
        }
    }

    stbi_image_free(data);
    return image;
}

Image4u ReadImage4u(const std::filesystem::path& filename, bool non_color, std::function<Image4u::Type(Image4u::Type)> transform)
{
    stbi_set_flip_vertically_on_load(true);
    stbi_ldr_to_hdr_gamma(non_color ? 1.0f : 2.2f);

    int32 width, height;
    int32 components_per_pixel;
    uint8* data = stbi_load(filename.string().c_str(), &width, &height, &components_per_pixel, STBI_rgb_alpha);

    if (!data)
    {
        std::cerr << "Failed to read image4: " << filename.string().c_str() << std::endl;
        return {};
    }

    constexpr int32 stride = STBI_rgb_alpha;
    Image4u image(width, height);

    if (width * height > 64 * 1024)
    {
        if (transform)
        {
            ParallelFor(0, width * height, [&](int32 i) {
                image[i] = transform(
                    Max(Vector4<uint8>{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2],
                                        (components_per_pixel == STBI_rgb_alpha ? data[stride * i + 3] : uint8(1)) },
                        Vector4<uint8>::zero)
                );
            });
        }
        else
        {
            ParallelFor(0, width * height, [&](int32 i) {
                image[i] =
                    Max(Vector4<uint8>{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2],
                                        (components_per_pixel == STBI_rgb_alpha ? data[stride * i + 3] : uint8(1)) },
                        Vector4<uint8>::zero);
            });
        }
    }
    else
    {
        if (transform)
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] = transform(
                    Max(Vector4<uint8>{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2],
                                        (components_per_pixel == STBI_rgb_alpha ? data[stride * i + 3] : uint8(1)) },
                        Vector4<uint8>::zero)
                );
            }
        }
        else
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] =
                    Max(Vector4<uint8>{ data[stride * i + 0], data[stride * i + 1], data[stride * i + 2],
                                        (components_per_pixel == STBI_rgb_alpha ? data[stride * i + 3] : uint8(1)) },
                        Vector4<uint8>::zero);
            }
        }
    }

    stbi_image_free(data);
    return image;
}

void WriteImage(const Image3& image, const std::filesystem::path& filename, ImageWriteCallback* callback)
{
    stbi_flip_vertically_on_write(true);

    std::string extension = filename.extension().string();
    if (extension == ".hdr")
    {
        stbi_write_hdr(filename.string().c_str(), image.width, image.height, 3, &image[0].x);
    }
    else if (extension == ".jpg" || extension == ".png")
    {
        std::vector<uint8> pixels(image.width * image.height * 3);

        if (image.width * image.height > 64 * 1024)
        {
            ParallelFor(0, image.width * image.height, [&](int32 i) {
                Vec3 mapped = callback({ image[i].x, image[i].y, image[i].z });

                pixels[i * 3 + 0] = uint8(std::min(std::clamp(mapped[0], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 1] = uint8(std::min(std::clamp(mapped[1], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 2] = uint8(std::min(std::clamp(mapped[2], 0.0f, 1.0f) * 256.0, 255.0));
            });
        }
        else
        {
            for (int32 i = 0; i < image.width * image.height; ++i)
            {
                Vec3 mapped = callback({ image[i].x, image[i].y, image[i].z });

                pixels[i * 3 + 0] = uint8(std::min(std::clamp(mapped[0], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 1] = uint8(std::min(std::clamp(mapped[1], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 2] = uint8(std::min(std::clamp(mapped[2], 0.0f, 1.0f) * 256.0, 255.0));
            }
        }

        if (extension == ".jpg")
        {
            stbi_write_jpg(filename.string().c_str(), image.width, image.height, 3, &pixels[0], 100);
        }
        else
        {
            stbi_write_png(filename.string().c_str(), image.width, image.height, 3, &pixels[0], image.width * 3);
        }
    }
    else
    {
        std::cerr << "Faild to write image, extention not supported: " << extension << std::endl;
        std::cerr << "Supported extensions: .jpg .png .hdr" << std::endl;
    }
}

void WriteImage(const Image1& image, const std::filesystem::path& filename, ImageWriteCallback* callback)
{
    stbi_flip_vertically_on_write(true);

    std::string extension = filename.extension().string();
    if (extension == ".hdr")
    {
        stbi_write_hdr(filename.string().c_str(), image.width, image.height, 1, &image[0]);
    }
    else if (extension == ".jpg" || extension == ".png")
    {
        std::vector<uint8> pixels(image.width * image.height * 3);

        if (image.width * image.height > 64 * 1024)
        {
            ParallelFor(0, image.width * image.height, [&](int32 i) {
                Vec3 mapped = callback({ image[i], image[i], image[i] });

                pixels[i * 3 + 0] = uint8(std::min(std::clamp(mapped[0], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 1] = uint8(std::min(std::clamp(mapped[1], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 2] = uint8(std::min(std::clamp(mapped[2], 0.0f, 1.0f) * 256.0, 255.0));
            });
        }
        else
        {
            for (int32 i = 0; i < image.width * image.height; ++i)
            {
                Vec3 mapped = callback({ image[i], image[i], image[i] });

                pixels[i * 3 + 0] = uint8(std::min(std::clamp(mapped[0], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 1] = uint8(std::min(std::clamp(mapped[1], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 2] = uint8(std::min(std::clamp(mapped[2], 0.0f, 1.0f) * 256.0, 255.0));
            }
        }

        if (extension == ".jpg")
        {
            stbi_write_jpg(filename.string().c_str(), image.width, image.height, 3, &pixels[0], 100);
        }
        else
        {
            stbi_write_png(filename.string().c_str(), image.width, image.height, 3, &pixels[0], image.width * 3);
        }
    }
    else
    {
        std::cerr << "Faild to write image, extention not supported: " << extension << std::endl;
        std::cerr << "Supported extensions: .jpg .png .hdr" << std::endl;
    }
}

void WriteImage(const Image3u& image, const std::filesystem::path& filename)
{
    stbi_flip_vertically_on_write(true);

    std::string extension = filename.extension().string();
    if (extension == ".jpg")
    {
        stbi_write_jpg(filename.string().c_str(), image.width, image.height, 3, image.data.get(), 100);
    }
    else if (extension == ".png")
    {
        stbi_write_png(filename.string().c_str(), image.width, image.height, 3, image.data.get(), image.width * 3);
    }
    else
    {
        std::cerr << "Faild to write image, extention not supported: " << extension << std::endl;
        std::cerr << "Supported extensions: .jpg .png" << std::endl;
    }
}

void WriteImage(const Image1u& image, const std::filesystem::path& filename)
{
    stbi_flip_vertically_on_write(true);

    std::string extension = filename.extension().string();
    if (extension == ".jpg" || extension == ".png")
    {
        std::vector<uint8> pixels(image.width * image.height * 3);

        if (image.width * image.height > 64 * 1024)
        {
            ParallelFor(0, image.width * image.height, [&](int32 i) {
                pixels[i * 3 + 0] = image[i];
                pixels[i * 3 + 1] = image[i];
                pixels[i * 3 + 2] = image[i];
            });
        }
        else
        {
            for (int32 i = 0; i < image.width * image.height; ++i)
            {
                pixels[i * 3 + 0] = image[i];
                pixels[i * 3 + 1] = image[i];
                pixels[i * 3 + 2] = image[i];
            }
        }

        if (extension == ".jpg")
        {
            stbi_write_jpg(filename.string().c_str(), image.width, image.height, 3, &pixels[0], 100);
        }
        else
        {
            stbi_write_png(filename.string().c_str(), image.width, image.height, 3, &pixels[0], image.width * 3);
        }
    }
    else
    {
        std::cerr << "Faild to write image, extention not supported: " << extension << std::endl;
        std::cerr << "Supported extensions: .jpg .png" << std::endl;
    }
}

} // namespace wak
