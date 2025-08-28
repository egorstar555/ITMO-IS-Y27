package com.Starygin.service;

import com.Starygin.service.dto.PetDto;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.kafka.requestreply.ReplyingKafkaTemplate;
import org.springframework.kafka.requestreply.RequestReplyFuture;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

@Service
public class PetServiceKafka {
    private final ObjectMapper mapper;

    private final ReplyingKafkaTemplate<String, Object, Object> replyingKafkaTemplate;

    @Autowired
    public PetServiceKafka(ObjectMapper mapper, ReplyingKafkaTemplate<String, Object, Object> replyingPetTemplate) {
        this.mapper = mapper;
        this.replyingKafkaTemplate = replyingPetTemplate;
    }

    public List<PetDto> findAll() throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        return mapper.readValue(sendRequestAndReceiveResponse("pets.findAll", ""), new TypeReference<List<PetDto>>() {
        });
    }

    public List<PetDto> findByName(String name) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        return mapper.readValue(sendRequestAndReceiveResponse("pets.findByName", name), new TypeReference<List<PetDto>>() {
        });
    }

    public PetDto findOne(Long id) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        String request = mapper.writeValueAsString(id);

        return mapper.readValue(sendRequestAndReceiveResponse("pets.findOne", request), PetDto.class);
    }

    public PetDto create(PetDto petDto) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        String request = mapper.writeValueAsString(petDto);

        return mapper.readValue(sendRequestAndReceiveResponse("pets.create-pet", request), PetDto.class);
    }

    public void delete(Long id) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        String request = mapper.writeValueAsString(id);

        sendRequestAndReceiveResponse("pets.delete-pet", request);
    }

    public PetDto updatePet(PetDto petDto) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        String request = mapper.writeValueAsString(petDto);

        return mapper.readValue(sendRequestAndReceiveResponse("pets.update-pet", request), PetDto.class);
    }

    private String sendRequestAndReceiveResponse(String topic, Object payload) throws ExecutionException, InterruptedException, TimeoutException {
        ProducerRecord<String, Object> record = new ProducerRecord<>(topic, payload);
        RequestReplyFuture<String, Object, Object> replyFuture = replyingKafkaTemplate.sendAndReceive(record);
        ConsumerRecord<String, Object> response = replyFuture.get(10, TimeUnit.SECONDS);

        return response.value().toString();
    }
}
