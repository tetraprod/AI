#!/bin/bash
set -e

# Build a minimal Debian ISO including the ChatGPT desktop assistant.
# Requires: debootstrap, live-build, xorriso, root privileges.

if [[ $EUID -ne 0 ]]; then
  echo "Run this script as root" >&2
  exit 1
fi

WORKDIR=$(pwd)/debian_chatgpt
mkdir -p "$WORKDIR"
cd "$WORKDIR"

lb config \
  --distribution bookworm \
  --debian-installer live \
  --packages "python3 python3-pip openbox"

# Copy the assistant into the live image
mkdir -p config/includes.chroot/opt/chatgpt
cp -r ../ai_gui config/includes.chroot/opt/chatgpt/

# Install openai in the chroot at build time
mkdir -p config/hooks/normal
cat <<'HOOK' > config/hooks/normal/99-install-openai.hook.chroot
#!/bin/sh
set -e
pip3 install openai
HOOK
chmod +x config/hooks/normal/99-install-openai.hook.chroot

# Add desktop entry
mkdir -p config/includes.chroot/usr/share/applications
cp ../ai_gui/chatgpt.desktop config/includes.chroot/usr/share/applications/

# Autostart the assistant for the live user
mkdir -p config/includes.chroot/etc/xdg/autostart
cp ../ai_gui/chatgpt.desktop config/includes.chroot/etc/xdg/autostart/

lb build

mv live-image-amd64.hybrid.iso ../chatgpt_debian.iso
echo "ISO created at $(pwd)/../chatgpt_debian.iso"

