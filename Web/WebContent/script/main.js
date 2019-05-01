function showpic() {
    var reads= new FileReader();
    f=document.getElementById('file').files[0];
    reads.readAsDataURL(f);
    reads.onload=function (e) {
    document.getElementById('show').src=this.result;
    };
}