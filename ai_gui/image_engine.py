from PIL import Image, ImageDraw, ImageFont


def create_image(text: str, path: str, size=(512, 512)):
    """Create a simple image with the given text."""
    img = Image.new('RGB', size, color='white')
    draw = ImageDraw.Draw(img)
    try:
        font = ImageFont.truetype('arial.ttf', 16)
    except IOError:
        font = ImageFont.load_default()
    draw.text((10, 10), text, fill='black', font=font)
    img.save(path)
