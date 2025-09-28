#include "environment/ScreenshotHandler.h"

#include "Handlers.h"
#include "config/Misc.h"
#include "environment/Environment.h"
#include "environment/x11/AtomID.h"
#include "log/Logger.h"

#include <Imlib2.h>
#include <cstring>

namespace {
  int get_shift_from_mask(unsigned long mask) {
    int shift = 0;
    while (!(mask & 1)) {
      mask >>= 1;
      shift++;
    }
    return shift;
  }

  void get_rgb_from_pixel(XImage* image,
                          unsigned long pixel,
                          unsigned char* r,
                          unsigned char* g,
                          unsigned char* b) {
    *r = (pixel & image->red_mask) >> get_shift_from_mask(image->red_mask);
    *g = (pixel & image->green_mask) >> get_shift_from_mask(image->green_mask);
    *b = (pixel & image->blue_mask) >> get_shift_from_mask(image->blue_mask);
  }

} // namespace

namespace ymwm::environment {

  ScreenshotHandler&
  ScreenshotHandler::add(const std::array<int, 2ul>& coords) noexcept {
    if (not m_start_coords) {
      m_start_coords = coords;
      return *this;
    }

    if (not m_end_coords) {
      m_end_coords = coords;
      return *this;
    }

    return *this;
  }

  void ScreenshotHandler::make(Environment& e) noexcept {
    if (not m_start_coords or not m_end_coords) {
      return;
    }

    auto xs = m_start_coords->at(0);
    auto ys = m_start_coords->at(1);
    auto xe = m_end_coords->at(0);
    auto ye = m_end_coords->at(1);
    auto x = std::min(xs, xe);
    auto y = std::min(ys, ye);

    auto width = m_end_coords->at(0) - m_start_coords->at(0);
    if (0 > width) {
      width *= -1;
    }
    auto height = m_end_coords->at(1) - m_start_coords->at(1);
    if (0 > height) {
      height *= -1;
    }

    auto* display = e.handlers().display;
    auto root_window = e.handlers().root_window;
    auto screen = e.handlers().screen;

    std::unique_ptr<XImage, std::function<void(XImage*)>> image(
        XGetImage(
            display, root_window, x, y, width, height, AllPlanes, ZPixmap),
        [](XImage* ptr) { XDestroyImage(ptr); });

    if (!image) {
      log::Logger::error("Failed to capture screen\n");
      return;
    }

    imlib_context_set_display(display);
    imlib_context_set_visual(DefaultVisual(display, screen));
    imlib_context_set_colormap(DefaultColormap(display, screen));

    Imlib_Image imlib_image = imlib_create_image(image->width, image->height);
    if (!imlib_image) {
      log::Logger::error("Failed to create Imlib2 image\n");
      return;
    }

    imlib_context_set_image(imlib_image);

    DATA32* imlib_data = imlib_image_get_data();
    for (int y = 0; y < image->height; ++y) {
      for (int x = 0; x < image->width; ++x) {
        unsigned long pixel = XGetPixel(image.get(), x, y);
        unsigned char r, g, b;
        get_rgb_from_pixel(image.get(), pixel, &r, &g, &b);
        imlib_data[y * image->width + x] = (r << 16) | (g << 8) | b;
      }
    }

    imlib_image_set_format("png");

    reset();

    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time_t);
    std::ostringstream filename;
    filename << "screenshot-" << std::put_time(&now_tm, "%Y-%m-%d-%H:%M:%S")
             << ".png";

    m_screenshot_path = ymwm::config::misc::screenshots_folder / filename.str();

    imlib_save_image(m_screenshot_path.c_str());

    // Need this path update, so to conform to URI.
    m_screenshot_path = "file://" + m_screenshot_path.string();

    std::size_t image_size = image->width * image->height;
    m_screenshot.clear();
    m_screenshot.reserve(image_size);
    std::memcpy(m_screenshot.data(), imlib_data, image_size);

    // Notify X11 that clipboard is occupied.
    XSetSelectionOwner(display,
                       e.handlers().atoms.at(AtomID::Clipboard),
                       root_window,
                       CurrentTime);

    imlib_free_image();

    m_start_coords = m_end_coords = std::nullopt;
  }

  bool ScreenshotHandler::has_screenshot() const noexcept {
    return not m_screenshot.empty() and not m_screenshot_path.empty();
  }

  const std::vector<std::uint32_t>&
  ScreenshotHandler::screenshot() const noexcept {
    return m_screenshot;
  }

  const std::filesystem::path&
  ScreenshotHandler::screenshot_path() const noexcept {
    return m_screenshot_path;
  }

  void ScreenshotHandler::reset() noexcept {
    m_screenshot.clear();
    m_screenshot_path.clear();
  }
} // namespace ymwm::environment
