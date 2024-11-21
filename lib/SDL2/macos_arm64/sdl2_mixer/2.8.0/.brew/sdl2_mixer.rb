class Sdl2Mixer < Formula
  desc "Sample multi-channel audio mixer library"
  homepage "https://github.com/libsdl-org/SDL_mixer"
  url "https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.0/SDL2_mixer-2.8.0.tar.gz"
  sha256 "1cfb34c87b26dbdbc7afd68c4f545c0116ab5f90bbfecc5aebe2a9cb4bb31549"
  license "Zlib"

  # This formula uses a file from a GitHub release, so we check the latest
  # release version instead of Git tags.
  livecheck do
    url :stable
    regex(/release[._-]v?(\d+(?:\.\d+)+)/i)
    strategy :github_latest
  end

  head do
    url "https://github.com/libsdl-org/SDL_mixer.git", branch: "main"

    depends_on "autoconf" => :build
    depends_on "automake" => :build
    depends_on "libtool" => :build
  end

  depends_on "pkg-config" => :build
  depends_on "flac"
  depends_on "fluid-synth"
  depends_on "game-music-emu"
  depends_on "libvorbis"
  depends_on "libxmp"
  depends_on "mpg123"
  depends_on "opusfile"
  depends_on "sdl2"
  depends_on "wavpack"

  def install
    inreplace "SDL2_mixer.pc.in", "@prefix@", HOMEBREW_PREFIX

    if build.head?
      mkdir "build"
      system "./autogen.sh"
    end

    system "./configure", *std_configure_args,
      "--enable-music-wave",
      "--enable-music-mod",
      "--enable-music-mod-xmp",
      "--disable-music-mod-xmp-shared",
      "--disable-music-mod-modplug",
      "--enable-music-midi",
      "--enable-music-midi-fluidsynth",
      "--disable-music-midi-fluidsynth-shared",
      "--disable-music-midi-native",
      "--disable-music-midi-timidity",
      "--enable-music-ogg",
      "--enable-music-ogg-vorbis",
      "--disable-music-ogg-vorbis-shared",
      "--disable-music-ogg-stb",
      "--disable-music-ogg-tremor",
      "--enable-music-flac",
      "--enable-music-flac-libflac",
      "--disable-music-flac-libflac-shared",
      "--disable-music-flac-drflac",
      "--enable-music-mp3",
      "--enable-music-mp3-mpg123",
      "--disable-music-mp3-mpg123-shared",
      "--disable-music-mp3-minimp3",
      "--enable-music-opus",
      "--disable-music-opus-shared",
      "--enable-music-gme",
      "--disable-music-gme-shared",
      "--enable-music-wavpack",
      "--enable-music-wavpack-dsd",
      "--disable-music-wavpack-shared"

    system "make", "install"
  end

  test do
    (testpath/"test.c").write <<~EOS
      #include <stdlib.h>
      #include <SDL2/SDL_mixer.h>

      int main()
      {
          const int INIT_FLAGS = MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS | MIX_INIT_WAVPACK;
          int success = Mix_Init(INIT_FLAGS);
          Mix_Quit();
          return success == INIT_FLAGS ? EXIT_SUCCESS : EXIT_FAILURE;
      }
    EOS
    system ENV.cc, "-I#{Formula["sdl2"].opt_include}/SDL2",
           "test.c", "-L#{lib}", "-lSDL2_mixer", "-o", "test"
    system "./test"
  end
end
