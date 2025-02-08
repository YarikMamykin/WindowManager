FROM archlinux:base

RUN pacman -Syy && \
		pacman-key --init && \
		pacman-key --populate archlinux && \
		pacman -Syy && pacman -S --noconfirm archlinux-keyring && \
		pacman -S --noconfirm cmake git make clang gcc findutils libx11 openssh xorg-setxkbmap imlib2 libheif libid3tag openjpeg2 libjxl libspectre librsvg cairo libwebp
