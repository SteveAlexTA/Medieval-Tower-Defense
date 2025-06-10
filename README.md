# Medieval Tower Defense
Medieval Tower Defense là 1 game thuộc thể loại Tower Defense. Mục tiêu bảo vệ thành khỏi những đợt tấn công của những kẻ thù đến từ cánh cổng địa ngục sử dụng những loại tháp trụ và vũ khí để sống sót và không để kẻ địch lọt qua.  

![image](https://github.com/user-attachments/assets/88689bc9-3871-4e6a-ade0-058c1594a8c7)
![image](https://github.com/user-attachments/assets/f2b73d72-2968-4233-8d81-3aa4f93be2a5)

- Gameplay Video: Coming soon...
- Thể loại: Tower Defense
- Nền tảng: Windows
- Ngôn ngữ lập trình/ thư viện sử dụng: C++ / SDL2 (Simple DirectMedia Layer)
- Tóm tắt: Người chơi sẽ phải phòng thủ căn cứ của mình trước những đợt tấn công của kẻ địch bằng cách đặt những tòa tháp, mỗi tòa tháp với các chỉ số khác nhau về tốc độ bắn và sát thương. Người chơi bắt đầu với 1 số tiền cho sẵn, sử dụng tiền để mua và đặt tháp chống lại những đợt tấn công của kẻ địch với các loại kẻ địch khác nhau về hình dạng, máu, tốc độ di chuyển. Độ khó tăng dần theo từng đợt tấn công. Nếu để kẻ địch lọt qua và căn cứ bị mất hết mạng thì trò chơi kết thúc.
# Cách tải và chơi game 
- Tải game: Tải file zip tại mục https://github.com/SteveAlexTA/Medieval-Tower-Defense/releases/tag/published, giải nén và chạy file Tower Defense Game SDL.exe để chơi game (lưu ý không phải file exe.recipe).
- Tải game & mã nguồn: Cài đặt <a href="https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/msvc2019/index.php" target="_blank">SDL2 và Visual Studio</a>, sau đó <a href="https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository" target="_blank">clone repo</a> này về và chạy game cùng source code qua file Medieval Tower Defense.sln.
# Lối chơi và logic của game
Người chơi sẽ phải phòng thủ căn cứ của mình trước những đợt tấn công của kẻ địch bằng cách đặt những tòa tháp với các chỉ số khác nhau. Người chơi dùng tiền để đặt tháp bắn lại các đợt tấn công của kẻ địch, mỗi kẻ địch giết được sẽ nhận được số tiền tương ứng với từng loại kẻ địch. Nếu có quá nhiều kẻ địch lọt qua và căn cứ bị mất hết 5 mạng thì trò chơi kết thúc. Mục tiêu sống sót qua 20 đợt tấn công của kẻ địch với độ khó tăng dần theo mỗi đợt.
- Sử dụng kéo thả để tương tác và đặt tháp.
- ESC để thoát trạng thái chọn tháp / nâng cấp.
- Cẩn thận tính toán tiền để đặt và nâng cấp tòa tháp của mình.
- Chọn vị trí hợp lí để đặt tháp, sau đó ngồi lại và theo dõi cuộc chiến diễn ra.
# Tính năng
- 4 loại kẻ địch với các chỉ số khác nhau (Goblin, Skeleton, Demon, Dragon).
- 3 loại tháp với các chỉ số khác nhau (Archer Tower, Cannon Tower, Lightning Tower).
- Hệ thống tiền với tiền thưởng cho mỗi kẻ địch bị giết, tiền đặt tháp và bán tháp.
- Hệ thống các đợt tấn công với 20 đợt tấn công và độ khó tăng dần theo từng đợt.
- Hệ thống xử lý va chạm khi tháp bắn vào kẻ địch và tìm đường đi cho kẻ địch trên bản đồ.
## Thuật toán sử dụng trong game (Mô phỏng)
### Tìm kiếm đường đi Greedy đơn giản 
Dự kiến cải tiến sang A*
### Tính khoảng cách kẻ địch (IsEnemyInRange)
![DemonstrateEnemyisinRange](https://github.com/user-attachments/assets/7bcc745e-6d54-4ccf-bfe1-56c61e603e72)
### Xử lí va chạm 
![DemonstrateCollisionDetection](https://github.com/user-attachments/assets/a3b07881-1b0b-4dad-bf4e-f25c37b6a8d1)
## Các tính năng sắp ra mắt:
- Thêm các loại tháp mới.
- Nâng cấp UI.
- Thêm độ khó và bản đồ mới.
- Boss fight.
# Tài liệu tham khảo
- Tham khảo code mẫu trên <a href ="https://lazyfoo.net/tutorials/SDL//" target="_blank">Lazy Foo' Productions</a>.
- Thao khảo code mẫu [johnmryan/TowerDefense](https://github.com/johnmryan/TowerDefense) trên GitHub.
- Hình ảnh, tài nguyên game được lấy từ <a href="https://skyel13.itch.io/simple-tower-defense" target="_blank">Skyel Tower Defense</a> và <a href="https://itch.io/" target="_blank">itch.io</a>.
- Hiệu ứng âm thanh được lấy từ <a href="https://pixabay.com/" target="_blank">Pixabay</a>.
