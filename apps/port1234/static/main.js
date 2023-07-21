
// https://www.youtube.com/watch?v=nF9riePnm80

$ (document).ready(function(){
    $('.btn').click(function(){
        $.ajax({
            url: '',
            type: 'get',
            contentType: 'application/json',
            data: {
                button_text: $(this).text()
            },
            sucess: function(response){
                $('.btn').text(response.seconds)
            }
        })
    })

})