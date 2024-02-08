import pygame
import pygame_gui
import socket
import json
from ipaddress import ip_address

# Initialize Pygame and Pygame GUI
pygame.init()
pygame.joystick.init()
pygame.display.set_caption("Robohacks Controls")
window_surface = pygame.display.set_mode((800, 600), pygame.RESIZABLE)
background = pygame.Surface((800, 600))
background.fill(pygame.Color('#000000'))

class DigitalButton(pygame.sprite.Sprite):
    def __init__(self, image, position, key):
        super().__init__()
        self.original_image = image
        self.image = image
        self.rect = image.get_rect(center=position)
        self.pressed_image = pygame.transform.scale(self.original_image, (int(self.rect.width * 0.9), int(self.rect.height * 0.9)))
        self.key = key
        self.is_pressed = False

    def update(self, pressed):
        self.is_pressed = pressed
        self.image = self.pressed_image if self.is_pressed else self.original_image  # Change image based on pressed state
        # TODO update socket

    def draw(self, surface):
        surface.blit(self.image, self.rect)

class AnalogStick(pygame.sprite.Sprite):
    def __init__(self, image, position, axis1, axis2):
        super().__init__()
        self.img_static = pygame.image.load('sprites/controller/analog_stick.png')
        self.img_dynamic = pygame.image.load('sprites/controller/analog_stick_moving.png')
        self.rect_static = self.img_static.get_rect()
        self.rect_dynamic = self.img_dynamic.get_rect()
        self.rect_static.center = position
        self.rect_dynamic.center = position
        self.value_x = 0.0
        self.value_y = 0.0
        self.axis1 = axis1
        self.axis2 = axis2

    def update(self, value_x, value_y):
        self.value_x = value_x
        self.value_y = value_y
        self.rect_dynamic.centerx = self.rect_static.centerx + value_x*50
        self.rect_dynamic.centery = self.rect_static.centery + value_y*50
        #TODO: update socket

    def draw(self, surface):

        surface.blit(self.img_static, self.rect_static)
        surface.blit(self.img_dynamic, self.rect_dynamic)


# Create a GUI manager
manager = pygame_gui.UIManager((800, 600))

# Create GUI elements
ip_text_entry = pygame_gui.elements.UITextEntryLine(relative_rect=pygame.Rect((10, 10), (140, 50)), manager=manager)
connect_button = pygame_gui.elements.UIButton(relative_rect=pygame.Rect((160, 10), (100, 50)), text='Connect', manager=manager)
status_text = pygame_gui.elements.UILabel(relative_rect=pygame.Rect((300, 10), (300, 50)), text='Status: Disconnected', manager=manager)
controller_text = pygame_gui.elements.UILabel(relative_rect=pygame.Rect((300, 250), (200, 50)), text='Controller Not Found', manager=manager)
controller_text.hide()
status_led = pygame.Rect(275, 25, 20, 20)  # Colored LED
# Create the toggle button for input mode
input_mode_button = pygame_gui.elements.UIButton(
    relative_rect=pygame.Rect((650, 10), (140, 50)),
    text='Mouse/Keyboard',
    manager=manager
)

# Variable to track input mode (True for Mouse/Keyboard, False for Controller)
input_mode = True  # Default to Mouse/Keyboard

# Controller and keyboard mapping
joystick = None
if pygame.joystick.get_count() > 0:
    joystick = pygame.joystick.Joystick(0)
    joystick.init()

# Map buttons and analogs
controller_input_map = {
    "leftstick": AnalogStick(None,(300,400), 0, 1),
    "rightstick": AnalogStick(None,(500,400), 2, 3),
    "dir_up" : DigitalButton(pygame.image.load('sprites/controller/dir_up.png'), (125,200), 11),
    "dir_down" : DigitalButton(pygame.image.load('sprites/controller/dir_down.png'), (125,300), 12),
    "dir_left" : DigitalButton(pygame.image.load('sprites/controller/dir_left.png'), (75,250), 13),
    "dir_right" : DigitalButton(pygame.image.load('sprites/controller/dir_right.png'), (175,250), 14),
    "btn_y" : DigitalButton(pygame.image.load('sprites/controller/btn_y.png'), (675,200), 3),
    "btn_a" : DigitalButton(pygame.image.load('sprites/controller/btn_a.png'), (675,300), 0),
    "btn_x" : DigitalButton(pygame.image.load('sprites/controller/btn_x.png'), (625,250), 2),
    "btn_b" : DigitalButton(pygame.image.load('sprites/controller/btn_b.png'), (725,250), 1)
}

keyboard_input_map = {
    "key_up" : DigitalButton(pygame.image.load('sprites/keyboard/key_up.png'), (600,390), pygame.K_UP),
    "key_down" : DigitalButton(pygame.image.load('sprites/keyboard/key_down.png'), (600,450), pygame.K_DOWN),
    "key_left" : DigitalButton(pygame.image.load('sprites/keyboard/key_left.png'), (480,450), pygame.K_LEFT),
    "key_right" : DigitalButton(pygame.image.load('sprites/keyboard/key_right.png'), (720,450), pygame.K_RIGHT),
    "key_w" : DigitalButton(pygame.image.load('sprites/keyboard/key_w.png'), (100,200), pygame.K_w),
    "key_a" : DigitalButton(pygame.image.load('sprites/keyboard/key_a.png'), (100,400), pygame.K_a),
    "key_s" : DigitalButton(pygame.image.load('sprites/keyboard/key_s.png'), (250,200), pygame.K_s),
    "key_d" : DigitalButton(pygame.image.load('sprites/keyboard/key_d.png'), (250,400), pygame.K_d)
}

clock = pygame.time.Clock()
is_running = True
esp_socket = None

def connect_esp(ip_input, port=80):
    try:
        _ = ip_address(ip_input)
    except ValueError:
        return False, "Invalid IP address"
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(5)
    s.connect((ip_input, port))
    return True, s

def send_updates(socket, input_map):
    float_num = 0
    obj_str = ""
    for key in input_map.keys():
        obj = input_map[key]
        if isinstance(obj, AnalogStick):
            float_num += 1
            obj_str += key + ","
            obj_str += str(obj.axis1) + ","
            obj_str += str(obj.axis2) + ","
        else:
            obj_str += key + ","
            obj_str += str(int(obj.is_pressed)) + ","
    full_string = str(float_num) + "," + obj_str[:-1]
    socket.sendall(bytes(full_string, "UTF-8"))

while is_running:
    time_delta = clock.tick(60)/1000.0

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            is_running = False

        elif event.type == pygame.VIDEORESIZE:
            # Update the window size
            window_surface = pygame.display.set_mode((event.w, event.h), pygame.RESIZABLE)
            background = pygame.Surface((event.w, event.h), pygame.SRCALPHA)
            background.fill(pygame.Color('#000000'))

        # Handle GUI events
        if event.type == pygame.USEREVENT:
            if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
                if event.ui_element == connect_button:
                    if esp_socket != None:
                        connect_button.set_text('Connect')
                        esp_socket.close()
                        esp_socket = None
                        status_text.set_text('Status: Disconnected')
                    else:
                        ip_input = ip_text_entry.get_text()
                        status_text.set_text(f"Status: Connecting to {ip_input}")
                        # status_text.
                        success, err = connect_esp(ip_input)
                        if not success:
                            status_text.set_text(f"Status: {err}")
                            esp_socket = None
                        else:
                            esp_socket = err
                            status_text.set_text(f"Status: Connected to {ip_input}")
                            connect_button.set_text('Disconnect')
                elif event.ui_element == input_mode_button:
                    # Toggle input mode
                    input_mode = not input_mode
                    input_mode_button.set_text('Controller' if not input_mode else 'Mouse/Keyboard')
                    if not input_mode:
                        if joystick == None and pygame.joystick.get_count() > 0:
                            joystick = pygame.joystick.Joystick(0)
                            joystick.init()
                        if joystick != None and pygame.joystick.get_count() == 0:
                            joystick = None
                        
                        if joystick == None:
                            controller_text.show()
                    else:
                        controller_text.hide()

                    # Now input_mode == True for Mouse/Keyboard, False for Controller

        # Update GUI
        manager.process_events(event)
    # Update keyboard states


    # Draw everything
    window_surface.blit(background, (0, 0))
    manager.update(time_delta)
    manager.draw_ui(window_surface)

    if input_mode:
        keys = pygame.key.get_pressed()
        mouse_pos = pygame.mouse.get_pos()
        # Update button_map based on keyboard inputs

        for elem in keyboard_input_map.values():
            elem.update(keys[elem.key])
            elem.draw(window_surface)
        send_updates(esp_socket, keyboard_input_map)
    else:
        # Update joystick/button states
        if joystick:
            for elem in controller_input_map.values():
                if isinstance(elem, AnalogStick):
                    elem.update(joystick.get_axis(elem.axis1), joystick.get_axis(elem.axis2))
                else:
                    elem.update(joystick.get_button(elem.key))
                elem.draw(window_surface)
        send_updates(esp_socket, controller_input_map)

    # Update the status LED color
    if 'Connected' in status_text.text:
        pygame.draw.rect(window_surface, (0, 255, 0), status_led)  # Green LED
    else:
        pygame.draw.rect(window_surface, (255, 0, 0), status_led)  # Red LED

    
    pygame.display.update()

pygame.quit()